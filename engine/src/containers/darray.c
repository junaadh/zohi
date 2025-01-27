#include "containers/darray.h"

#include "core/logger.h"
#include "core/zmemory.h"
#include "defines.h"

void *_darray_create(u64 length, u64 stride) {
    u64 header_size = DARRAY_FIELD_LENGTH * sizeof(u64);
    u64 array_size = length * stride;
    u64 *new_array = zallocate(header_size + array_size, MEMORY_TAG_DARRAY);

    zset_memory(new_array, 0, header_size + array_size);
    new_array[DARRAY_CAPACITY] = length;
    new_array[DARRAY_LENGTH] = 0;
    new_array[DARRAY_STRIDE] = stride;
    return (void *)(new_array + DARRAY_FIELD_LENGTH);
}

void _darray_destroy(void *array) {
    u64 *header = (u64 *)array - DARRAY_FIELD_LENGTH;
    u64 header_size = DARRAY_FIELD_LENGTH * sizeof(u64);
    u64 total_size = header_size + header[DARRAY_CAPACITY] * header[DARRAY_STRIDE];
    zfree(header, total_size, MEMORY_TAG_DARRAY);
}

u64 _darray_field_get(void *array, u64 field) {
    u64 *header = (u64 *)array - DARRAY_FIELD_LENGTH;
    return header[field];
}

void _darray_field_set(void *array, u64 field, u64 value) {
    u64 *header = (u64 *)array - DARRAY_FIELD_LENGTH;
    header[field] = value;
}

void *_darray_resize(void *array) {
    u64 length = darray_length(array);
    u64 stride = darray_stride(array);

    void *temp = _darray_create(
        (DARRAY_RESIZE_FACTOR * darray_capacity(array)),
        stride);
    zcopy_memory(temp, array, length * stride);

    _darray_field_set(temp, DARRAY_LENGTH, length);
    _darray_destroy(array);
    return temp;
}

void *_darray_push(void *array, const void *value_ptr) {
    u64 length = darray_length(array);
    u64 stride = darray_stride(array);
    if (length >= darray_capacity(array)) {
        _darray_resize(array);
    }

    u64 addr = (u64)array;
    addr += (length * stride);
    zcopy_memory((void *)addr, value_ptr, stride);
    _darray_field_set(array, DARRAY_LENGTH, length + 1);
    return array;
}

void _darray_pop(void *array, void *dest) {
    u64 length = darray_length(array);
    u64 stride = darray_stride(array);
    u64 addr = (u64)array;

    addr += ((length - 1) * stride);
    zcopy_memory(dest, (void *)addr, stride);
    _darray_field_set(array, DARRAY_LENGTH, length - 1);
}

void *_darray_pop_at(void *array, u64 idx, void *dest) {
    u64 length = darray_length(array);
    u64 stride = darray_stride(array);
    if (idx >= length) {
        ZERROR("Index out of bounds of this arraay! Length: %i, index: %i", length, idx);
        return array;
    }

    u64 addr = (u64)array;
    zcopy_memory(dest, (void *)(addr + (idx * stride)), stride);

    if (idx != length - 1) {
        zcopy_memory((void *)(addr + (idx * stride)), (void *)(addr + ((idx + 1) * stride)), stride * (length - idx));
    }

    _darray_field_set(array, DARRAY_LENGTH, length - 1);
    return array;
}

void *_darray_insert_at(void *array, u64 idx, void *value_ptr) {
    u64 length = darray_length(array);
    u64 stride = darray_stride(array);
    if (idx >= length) {
        ZERROR("Index out of bounds of this array! Length: %i, index: %i", length, idx);
        return array;
    }

    if (length >= darray_capacity(array)) {
        array = _darray_resize(array);
    }

    u64 addr = (u64)array;

    // if not on the last element. copy the rest outward
    if (idx != length - 1) {
        zcopy_memory((void *)(addr + ((idx + 1) * stride)), (void *)(addr + (idx * stride)), stride * (length - idx));
    }

    zcopy_memory((void *)(addr + (idx * stride)), value_ptr, stride);
    _darray_field_set(array, DARRAY_LENGTH, length + 1);
    return array;
}
