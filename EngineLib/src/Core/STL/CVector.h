#ifndef CVECTOR_HEADER
#define CVECTOR_HEADER
/**
 * @file
 * @author Krusto Stoyanov ( k.stoianov2@gmail.com )
 * @brief 
 * @version 1.0
 * @date 
 * 
 * @section LICENSE
 * MIT License
 * 
 * Copyright (c) 2024 Krusto
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * @section DESCRIPTION
 * 
 * CArray Header
 */

#ifdef __cplusplus
#include <cstdint>
namespace Engine::Containers
{
#else
#include <stdint.h>
#endif
/***********************************************************************************************************************
Macro Definitions
***********************************************************************************************************************/

/**
 * @def CVECTOR_INITIAL_CAPACITY
 * @brief The initial capacity of a vector.
 */
#define CVECTOR_INITIAL_CAPACITY 1u

/**
 * @def CVECTOR_RESIZE_FACTOR
 * @brief The resize factor of a vector.
 *
 * If the length of the array exceeds the capacity, the capacity is doubled.
 */
#define CVECTOR_RESIZE_FACTOR 2u

    /***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/

    /**
 * @struct CVectorT
 * @brief A vector.
 * 
 * A vector is a type of array that can grow in size as elements are added
 * to it. This is useful for storing collections of data when the number of
 * elements is not known until runtime.
 * 
 * @var length The number of elements in the vector.
 * @var capacity The maximum number of elements that the vector can hold.
 * @var elementSize The size of each element in the vector.
 * @var data The data of the vector.
 */
    typedef struct {
        size_t length;
        size_t capacity;
        size_t elementSize;
        int8_t* data;
    } CVectorT;

    /**
 * @typedef CVectorU32T
 * @brief A vector of uint32_t.
 */
    typedef CVectorT CvectorU32T;

    /**
 * @typedef CVectorI32T
 * @brief A vector of int32_t.
 */
    typedef CVectorT CVectorI32T;

    /**
 * @typedef CVectorU16T
 * @brief A vector of uint16_t.
 */
    typedef CVectorT CVectorU16T;

    /**
 * @typedef CVectorI16T
 * @brief A vector of int16_t.
 */
    typedef CVectorT CVectorI16T;

    /**
 * @typedef CVectorU8T
 * @brief A vector of uint8_t.
 */
    typedef CVectorT CVectorU8T;

    /**
 * @typedef CVectorI8T
 * @brief A vector of int8_t.
 */
    typedef CVectorT CVectorI8T;

    /***********************************************************************************************************************
Static functions declaration
***********************************************************************************************************************/
    /**
 * @brief Destroy a vector.
 * @param cvector[in] The vector to destroy.
 * @return None.
 */
    static void cvector_destroy(CVectorT* vector);

    /**
 * @brief Create a vector of uint32_t.
 * @return A pointer to the new vector.
 */
    static CVectorU32T* cvector_create_u32();

    /**
 * @brief Create a vector of int32_t.
 * @return A pointer to the new vector.
 */
    static CVectorI32T* cvector_create_i32();

    /**
 * @brief Create a vector of uint16_t.
 * @return A pointer to the new vector.
 */
    static CVectorU16T* cvector_create_u16();

    /**
 * @brief Create a vector of int16_t.
 * @return A pointer to the new vector.
 */
    static CVectorI16T* cvector_create_i16();

    /**
 * @brief Create a vector of uint8_t.
 * @return A pointer to the new vector.
 */
    static CVectorU8T* cvector_create_u8();

    /**
 * @brief Create a vector of int8_t.
 * @return A pointer to the new vector.
 */
    static CVectorI8T* cvector_create_i8();

    /**
 * @brief Create a vector of any type.
 * @param typeSize[in] The size of the type to be stored in the array.
 * @return A pointer to the new vector.
 */
    static CVectorT* cvector_create_generic(size_t typeSize);

    /**
 * @brief Push a new element to the end of the vector.
 * @param cvector[in] The vector.
 * @param value[in] The value to push.
 * @return None.
 */
    static void cvector_push_u32(CVectorU32T* vector, uint32_t value);

    /**
 * @brief Push a new element to the end of the vector.
 * @param cvector[in] The vector.
 * @param value[in] The value to push.
 * @return None.
 */
    static void cvector_push_i32(CVectorI32T* vector, int32_t value);

    /**
 * @brief Push a new element to the end of the vector.
 * @param cvector[in] The vector.
 * @param value[in] The value to push.
 * @return None.
 */
    static void cvector_push_u16(CVectorU16T* vector, uint16_t value);

    /**
 * @brief Push a new element to the end of the vector.
 * @param cvector[in] The vector.
 * @param value[in] The value to push.
 * @return None.
 */
    static void cvector_push_i16(CVectorI16T* vector, int16_t value);

    /**
 * @brief Push a new element to the end of the vector.
 * @param cvector[in] The vector.
 * @param value[in] The value to push.
 * @return None.
 */
    static void cvector_push_u8(CVectorU8T* vector, uint8_t value);

    /**
 * @brief Push a new element to the end of the vector.
 * @param cvector[in] The vector.
 * @param value[in] The value to push.
 * @return None.
 */
    static void cvector_push_i8(CVectorI8T* vector, int8_t value);

    /**
 * @brief Push a new element to the end of the vector.
 * @param cvector[in] The vector.
 * @param value[in] The value to push.
 * @return None.
 */
    static void cvector_push_generic(CVectorT* vector, void* value);

    /**
 * @brief Insert a new element at the given index in the vector.
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to insert.
 * @param value[in] The value to insert.
 * @return None.
 */
    static void cvector_insert_u32(CVectorU32T* vector, size_t index, uint32_t value);

    /**
 * @brief Insert a new element at the given index in the vector.
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to insert.
 * @param value[in] The value to insert.
 * @return None.
 */
    static void cvector_insert_i32(CVectorI32T* vector, size_t index, int32_t value);

    /**
 * @brief Insert a new element at the given index in the vector.
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to insert.
 * @param value[in] The value to insert.
 * @return None.
 */
    static void cvector_insert_u16(CVectorU16T* vector, size_t index, uint16_t value);

    /**
 * @brief Insert a new element at the given index in the vector.
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to insert.
 * @param value[in] The value to insert.
 * @return None.
 */
    static void cvector_insert_i16(CVectorI16T* vector, size_t index, int16_t value);

    /**
 * @brief Insert a new element at the given index in the vector.
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to insert.
 * @param value[in] The value to insert.
 * @return None.
 */
    static void cvector_insert_u8(CVectorU8T* vector, size_t index, uint8_t value);

    /**
 * @brief Insert a new element at the given index in the vector.
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to insert.
 * @param value[in] The value to insert.
 * @return None.
 */
    static void cvector_insert_i8(CVectorI8T* vector, size_t index, int8_t value);

    /**
 * @brief Insert a new element at the given index in the vector.
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to insert.
 * @param value[in] The value to insert.
 * @return None.
 */
    static void cvector_insert_generic(CVectorT* vector, size_t index, void* valuePtr);

    /**
 * @brief Get a value from the vector.
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return The value at the given index.
 */
    static uint32_t cvector_get_u32(CVectorU32T* vector, size_t index);

    /**
 * @brief Get a value from the vector, safely.
 *
 * This function is like cvector_get_u32, but it will return 0 if the index is out
 * of bounds of the vector.
 *
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return The value at the given index, or 0 if the index is out of bounds.
 */
    static uint32_t cvector_get_u32_safe(CVectorU32T* vector, size_t index);

    /**
 * @brief Get a value from the vector.
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return The value at the given index.
 */
    static int32_t cvector_get_i32(CVectorI32T* vector, size_t index);
    /**
 * @brief Get a value from the vector, safely.
 *
 * This function is like cvector_get_i32, but it will return 0 if the index is out
 * of bounds of the vector.
 *
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return The value at the given index, or 0 if the index is out of bounds.
 */
    static int32_t cvector_get_i32_safe(CVectorI32T* vector, size_t index);

    /**
 * @brief Get a pointer to a value in the vector.
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return A pointer to the value at the given index.
 */
    static uint32_t* cvector_get_u32_ptr(CVectorU32T* vector, size_t index);

    /**
 * @brief Get a pointer to a value in the vector, safely.
 *
 * This function is like cvector_get_u32_ptr, but it will return NULL if the index
 * is out of bounds of the vector.
 *
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return A pointer to the value at the given index, or NULL if the index is
 * out of bounds.
 */
    static uint32_t* cvector_get_u32_ptr_safe(CVectorU32T* vector, size_t index);

    /**
 * @brief Get a pointer to a value in the vector.
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return A pointer to the value at the given index.
 */
    static int32_t* cvector_get_i32_ptr(CVectorI32T* vector, size_t index);

    /**
 * @brief Get a pointer to a value in the vector, safely.
 *
 * This function is like cvector_get_i32_ptr, but it will return NULL if the index
 * is out of bounds of the vector.
 *
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return A pointer to the value at the given index, or NULL if the index is
 * out of bounds.
 */
    static int32_t* cvector_get_i32_ptr_safe(CVectorI32T* vector, size_t index);

    /**
 * @brief Get a value from the vector.
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return The value at the given index.
 */
    static uint16_t cvector_get_u16(CVectorU16T* vector, size_t index);

    /**
 * @brief Get a value from the vector, safely.
 *
 * This function is like cvector_get_u16, but it will return 0 if the index is out
 * of bounds of the vector.
 *
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return The value at the given index, or 0 if the index is out of bounds.
 */
    static uint16_t cvector_get_u16_safe(CVectorU16T* vector, size_t index);

    /**
 * @brief Get a value from the vector.
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return The value at the given index.
 */
    static int16_t cvector_get_i16(CVectorI16T* vector, size_t index);

    /**
 * @brief Get a value from the vector, safely.
 *
 * This function is like cvector_get_i16, but it will return 0 if the index is out
 * of bounds of the vector.
 *
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return The value at the given index, or 0 if the index is out of bounds.
 */
    static int16_t cvector_get_i16_safe(CVectorI16T* vector, size_t index);

    /**
 * @brief Get a pointer to a value in the vector.
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return A pointer to the value at the given index.
 */
    static uint16_t* cvector_get_u16_ptr(CVectorU16T* vector, size_t index);

    /**
 * @brief Get a pointer to a value in the vector, safely.
 *
 * This function is like cvector_get_u16_ptr, but it will return NULL if the index
 * is out of bounds of the vector.
 *
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return A pointer to the value at the given index, or NULL if the index is
 * out of bounds.
 */
    static uint16_t* cvector_get_u16_ptr_safe(CVectorU16T* vector, size_t index);

    /**
 * @brief Get a pointer to a value in the vector.
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return A pointer to the value at the given index.
 */
    static int16_t* cvector_get_i16_ptr(CVectorI16T* vector, size_t index);

    /**
 * @brief Get a pointer to a value in the vector, safely.
 *
 * This function is like cvector_get_i16_ptr, but it will return NULL if the index
 * is out of bounds of the vector.
 *
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return A pointer to the value at the given index, or NULL if the index is
 * out of bounds.
 */
    static int16_t* cvector_get_i16_ptr_safe(CVectorI16T* vector, size_t index);

    /**
 * @brief Get a value from the vector.
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return The value at the given index.
 */
    static uint8_t cvector_get_u8(CVectorU8T* vector, size_t index);

    /**
 * @brief Get a value from the vector.
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return The value at the given index.
 */
    static int8_t cvector_get_i8(CVectorI8T* vector, size_t index);

    /**
 * @brief Get a value from the vector, safely.
 *
 * This function is like cvector_get_i8, but it will return 0 if the index is out
 * of bounds of the vector.
 *
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return The value at the given index, or 0 if the index is out of bounds.
 */
    static int8_t cvector_get_i8_safe(CVectorI8T* vector, size_t index);

    /**
 * @brief Get a pointer to a value in the vector.
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return A pointer to the value at the given index.
 */
    static uint8_t* cvector_get_u8_ptr(CVectorU8T* vector, size_t index);

    /**
 * @brief Get a pointer to a value in the vector, safely.
 *
 * This function is like cvector_get_u8_ptr, but it will return NULL if the index
 * is out of bounds of the vector.
 *
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return A pointer to the value at the given index, or NULL if the index is
 * out of bounds.
 */
    static uint8_t* cvector_get_u8_ptr_safe(CVectorU8T* vector, size_t index);

    /**
 * @brief Get a pointer to a value in the vector.
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return A pointer to the value at the given index.
 */
    static int8_t* cvector_get_i8_ptr(CVectorI8T* vector, size_t index);

    /**
 * @brief Get a pointer to a value in the vector, safely.
 *
 * This function is like cvector_get_i8_ptr, but it will return NULL if the index
 * is out of bounds of the vector.
 *
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return A pointer to the value at the given index, or NULL if the index is
 * out of bounds.
 */
    static int8_t* cvector_get_i8_ptr_safe(CVectorI8T* vector, size_t index);

    /**
 * @brief Get a pointer to a value in the vector.
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return A pointer to the value at the given index.
 */
    static void* cvector_get_ptr(CVectorT* vector, size_t index);

    /**
 * @brief Get a pointer to a value in the vector, safely.
 *
 * This function is like cvector_get_ptr, but it will return NULL if the index
 * is out of bounds of the vector.
 *
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to get.
 * @return A pointer to the value at the given index, or NULL if the index is
 * out of bounds.
 */
    static void* cvector_get_ptr_safe(CVectorT* vector, size_t index);

    /**
 * @brief Resize the vector.
 * @param cvector[in] The vector.
 * @param newLength[in] The new length of the vector.
 */
    static void cvector_resize(CVectorT* vector, size_t newLength);

    /**
 * @brief Get a pointer to the first value in the vector.
 * @param cvector[in] The vector.
 * @return A pointer to the first value in the vector.
 */
    static void* cvector_front_ptr(CVectorT* vector);

    /**
 * @brief Get a pointer to the first value in the vector, safely.
 *
 * This function is like cvector_front_ptr, but it will return NULL if the dynamic
 * array is empty.
 *
 * @param cvector[in] The vector.
 * @return A pointer to the first value in the vector, or NULL if the
 * vector is empty.
 */
    static void* cvector_front_ptr_safe(CVectorT* vector);

    /**
 * @brief Get a pointer to the last value in the vector.
 * @param cvector[in] The vector.
 * @return A pointer to the last value in the vector.
 */
    static void* cvector_back_ptr(CVectorT* vector);

    /**
 * @brief Get a pointer to the last value in the vector, safely.
 *
 * This function is like cvector_back_ptr, but it will return NULL if the dynamic
 * array is empty.
 *
 * @param cvector[in] The vector.
 * @return A pointer to the last value in the vector, or NULL if the
 * vector is empty.
 */
    static void* cvector_back_ptr_safe(CVectorT* vector);

    /**
 * @brief Check if the vector is empty.
 * @param cvector[in] The vector.
 * @return TRUE if the vector is empty, FALSE otherwise.
 */
    static BOOL cvector_is_empty(CVectorT* vector);

    /**
 * @brief Get the length of the vector.
 * @param cvector[in] The vector.
 * @return The length of the vector.
 */
    static size_t cvector_length(CVectorT* vector);

    /**
 * @brief Get the capacity of the vector.
 * @param cvector[in] The vector.
 * @return The capacity of the vector.
 */
    static size_t cvector_capacity(CVectorT* vector);

    /**
 * @brief Erase an element from the vector.
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to erase.
 */
    static void cvector_erase(CVectorT* vector, size_t index);

    /**
 * @brief Erase an element from the vector, safely.
 *
 * This function is like cvector_erase, but it will do nothing if the index is out
 * of bounds of the vector.
 *
 * @param cvector[in] The vector.
 * @param index[in] The index of the element to erase.
 */
    static void cvector_erase_safe(CVectorT* vector, size_t index);

    /**
 * @brief Shrink the capacity of the vector to its length.
 * @param cvector[in] The vector.
 */
    static void cvector_shrink_to_fit(CVectorT* vector);

    /**
 * @brief Reserve space for a number of elements in the vector.
 * @param cvector[in] The vector.
 * @param newCapacity[in] The new capacity of the vector.
 */
    static void cvector_reserve(CVectorT* vector, size_t newCapacity);

    /**
 * @brief Pop a value from the vector.
 * @param cvector[in] The vector.
 * @return A pointer to the popped value.
 */
    static void* cvector_pop(CVectorT* vector);
    /**
 * @brief Pop a value from the vector, safely.
 *
 * This function is like cvector_pop, but it will return NULL if the vector
 * is empty.
 *
 * @param cvector[in] The vector.
 * @return A pointer to the popped value, or NULL if the vector is empty.
 */
    static void* cvector_pop_safe(CVectorT* vector);

    /***********************************************************************************************************************
Static functions implementation
***********************************************************************************************************************/

    inline static void cvector_insert_generic(CVectorT* vector, size_t index, void* valuePtr)
    {
        if (index < vector->length + 1)
        {
            cvector_resize(vector, darr->length + 1);
            if (index < vector->length)
            {
                void* resultPtr = NULL;
                if (NULL != vector->data)
                {
                    size_t len = (vector->length) - (index + 1);
                    void* next = (void*) &vector->data[index * vector->elementSize + darr->elementSize];
                    void* current = &vector->data[index * vector->elementSize];
                    resultPtr = CMEMCPY(next, current, len * vector->elementSize);
                    if (NULL != resultPtr)
                    {
                        CMEMCPY(&vector->data[index * vector->elementSize], valuePtr, darr->elementSize);
                    }
                }
            }
        }
    }

    inline static void cvector_insert_u32(CVectorU32T* vector, size_t index, uint32_t value)
    {
        cvector_insert_generic(vector, index, &value);
    }

    inline static void cvector_insert_i32(CVectorI32T* vector, size_t index, int32_t value)
    {
        cvector_insert_generic(vector, index, &value);
    }

    inline static void cvector_insert_u16(CVectorU16T* vector, size_t index, uint16_t value)
    {
        cvector_insert_generic(vector, index, &value);
    }

    inline static void cvector_insert_i16(CVectorI16T* vector, size_t index, int16_t value)
    {
        cvector_insert_generic(vector, index, &value);
    }

    inline static void cvector_insert_u8(CVectorU8T* vector, size_t index, uint8_t value)
    {
        cvector_insert_generic(vector, index, &value);
    }

    inline static void cvector_insert_i8(CVectorI8T* vector, size_t index, int8_t value)
    {
        cvector_insert_generic(vector, index, &value);
    }

    inline static void* cvector_get_ptr(CVectorT* vector, size_t index)
    {
        return &darr->data[index * darr->elementSize];
    }

    inline static void* cvector_get_ptr_safe(CVectorT* vector, size_t index)
    {
        void* result = NULL;
        if (NULL == cvector) {}
        else if (index < vector->length) { result = vector_get_ptr(darr, index); }
        return result;
    }

    inline static uint32_t cvector_get_u32(CVectorU32T* vector, size_t index)
    {
        return *((uint32_t*) darr_get_ptr(darr, index));
    }

    inline static uint32_t cvector_get_u32_safe(CVectorU32T* vector, size_t index)
    {
        uint32_t result = 0;
        if (NULL == cvector) {}
        else if (index < vector->length) { result = vector_get_u32(darr, index); }
        return result;
    }

    inline static int32_t cvector_get_i32(CVectorI32T* vector, size_t index)
    {
        return *((int32_t*) darr_get_ptr(darr, index));
    }

    inline static int32_t cvector_get_i32_safe(CVectorI32T* vector, size_t index)
    {
        int32_t result = 0;
        if (NULL == cvector) {}
        else if (index < vector->length) { result = vector_get_i32(darr, index); }
        return result;
    }

    inline static uint32_t* cvector_get_u32_ptr(CVectorU32T* vector, size_t index)
    {
        return ((uint32_t*) cvector_get_ptr(vector, index));
    }

    inline uint32_t* cvector_get_u32_ptr_safe(CVectorU32T* vector, size_t index)
    {
        uint32_t* result = NULL;
        if (NULL == cvector) {}
        else if (index < vector->length) { result = vector_get_u32_ptr(darr, index); }
        return result;
    }

    inline static int32_t* cvector_get_i32_ptr(CVectorI32T* vector, size_t index)
    {
        return ((int32_t*) cvector_get_ptr(vector, index));
    }

    inline int32_t* cvector_get_i32_ptr_safe(CVectorU32T* vector, size_t index)
    {
        int32_t* result = NULL;
        if (NULL == cvector) {}
        else if (index < vector->length) { result = vector_get_i32_ptr(darr, index); }
        return result;
    }

    inline static uint16_t cvector_get_u16(CVectorU16T* vector, size_t index)
    {
        return *((uint16_t*) darr_get_ptr(darr, index));
    }

    inline static int16_t cvector_get_i16(CVectorI16T* vector, size_t index)
    {
        return *((int16_t*) darr_get_ptr(darr, index));
    }

    inline static uint16_t* cvector_get_u16_ptr(CVectorU16T* vector, size_t index)
    {
        return ((uint16_t*) cvector_get_ptr(vector, index));
    }

    inline static int16_t* cvector_get_i16_ptr(CVectorI16T* vector, size_t index)
    {
        return ((int16_t*) cvector_get_ptr(vector, index));
    }

    inline static int16_t cvector_get_i16_safe(CVectorI16T* vector, size_t index)
    {
        int16_t result = 0;
        if (NULL == cvector) {}
        else if (index < vector->length) { result = vector_get_i16(darr, index); }
        return result;
    }

    inline static uint16_t* cvector_get_u16_ptr_safe(CVectorU16T* vector, size_t index)
    {
        uint16_t* result = NULL;
        if (NULL == cvector) {}
        else if (index < vector->length) { result = vector_get_u16_ptr(darr, index); }
        return result;
    }

    inline static int16_t* cvector_get_i16_ptr_safe(CVectorI16T* vector, size_t index)
    {
        int16_t* result = NULL;
        if (NULL == cvector) {}
        else if (index < vector->length) { result = vector_get_i16_ptr(darr, index); }
        return result;
    }

    inline static uint8_t cvector_get_u8(CVectorU8T* vector, size_t index)
    {
        return *((uint8_t*) darr_get_ptr(darr, index));
    }

    inline static int8_t cvector_get_i8(CVectorI8T* vector, size_t index)
    {
        return *((int8_t*) darr_get_ptr(darr, index));
    }

    inline static uint8_t* cvector_get_u8_ptr(CVectorU8T* vector, size_t index)
    {
        return ((uint8_t*) darr_get_ptr(darr, index));
    }

    inline static int8_t* cvector_get_i8_ptr(CVectorI8T* vector, size_t index)
    {
        return ((int8_t*) darr_get_ptr(darr, index));
    }

    inline static uint8_t cvector_get_u8_safe(CVectorU8T* vector, size_t index)
    {
        uint8_t result = 0;
        if (NULL == cvector) {}
        else if (index < vector->length) { result = vector_get_u8(darr, index); }
        return result;
    }

    inline static int8_t cvector_get_i8_safe(CVectorI8T* vector, size_t index)
    {
        int8_t result = 0;
        if (NULL == cvector) {}
        else if (index < vector->length) { result = vector_get_i8(darr, index); }
        return result;
    }

    inline static uint8_t* cvector_get_u8_ptr_safe(CVectorU8T* vector, size_t index)
    {
        uint8_t* result = NULL;
        if (NULL == cvector) {}
        else if (index < vector->length) { result = vector_get_u8_ptr(darr, index); }
        return result;
    }

    inline static int8_t* cvector_get_i8_ptr_safe(CVectorI8T* vector, size_t index)
    {
        int8_t* result = NULL;
        if (NULL == cvector) {}
        else if (index < vector->length) { result = vector_get_i8_ptr(darr, index); }
        return result;
    }

    inline static void* cvector_front_ptr(CVectorT* vector) { return darr_get_ptr(darr, 0); }

    inline static void* cvector_back_ptr(CVectorT* vector) { return darr_get_ptr(darr, darr->length - 1); }

    inline static void* cvector_front_ptr_safe(CVectorT* vector)
    {
        void* result = NULL;
        if (NULL == cvector) {}
        else if (vector->length > 0) { result = vector_get_ptr(darr, 0); }
        return result;
    }

    inline static void* cvector_back_ptr_safe(CVectorT* vector)
    {
        void* result = NULL;
        if (NULL == cvector) {}
        else if (vector->length > 0) { result = vector_get_ptr(darr, darr->length - 1); }
        return result;
    }

    inline static void cvector_push_u32(CVectorU32T* vector, uint32_t value) { darr_push_generic(darr, &value); }

    inline static void cvector_push_i32(CVectorI32T* vector, int32_t value) { darr_push_generic(darr, &value); }

    inline static void cvector_push_u16(CVectorU16T* vector, uint16_t value) { darr_push_generic(darr, &value); }

    inline static void cvector_push_i16(CVectorI16T* vector, int16_t value) { darr_push_generic(darr, &value); }

    inline static void cvector_push_u8(CVectorU8T* vector, uint8_t value) { darr_push_generic(darr, &value); }

    inline static void cvector_push_i8(CVectorI8T* vector, int8_t value) { darr_push_generic(darr, &value); }

    inline static void cvector_push_generic(CVectorT* vector, void* value)
    {
        cvector_resize(vector, darr->length + 1);
        CMEMCPY(&vector->data[vector->length * darr->elementSize - darr->elementSize], value, darr->elementSize);
    }

    inline static void cvector_push_ptr(CVectorT* vector, void* value)
    {
        cvector_resize(vector, darr->length + 1);
        CMEMCPY(&vector->data[vector->length * darr->elementSize - darr->elementSize], &value, darr->elementSize);
    }

    inline static BOOL cvector_is_empty(CVectorT* vector) { return (0u == darr->length); }

    inline static size_t cvector_length(CVectorT* vector) { return darr->length; }

    inline static size_t cvector_capacity(CVectorT* vector) { return darr->capacity; }

    inline static void cvector_resize(CVectorT* vector, size_t newLength)
    {
        if (newLength > vector->length)
        {
            if (newLength > vector->capacity)
            {
                int8_t* resultPtr;
                size_t newCapacity = newLength * CVECTOR_RESIZE_FACTOR;

                if (NULL == vector->data) { resultPtr = (int8_t*) CMALLOC(newCapacity * (vector->elementSize)); }
                else { resultPtr = (int8_t*) CREALLOC(vector->data, newCapacity * (vector->elementSize)); }
                if (NULL == resultPtr) { LOG_ERROR("Can not allocate cvector!\n"); }
                if (NULL != resultPtr)
                {
                    vector->data = resultPtr;
                    vector->length = newLength;
                    vector->capacity = newCapacity;
                }
            }
            else { vector->length = newLength; }
        }
        else { vector->length = newLength; }
    }

    inline static void cvector_destroy(CVectorT* vector)
    {
        CFREE(vector->data, vector->length);
        CFREE(cvector, sizeof(vector));
    }

    inline static CVectorU32T* cvector_create_u32() { return vector_create_generic(sizeof(uint32_t)); }

    inline static CVectorI32T* cvector_create_i32() { return vector_create_generic(sizeof(int32_t)); }

    inline static CVectorU16T* cvector_create_u16() { return vector_create_generic(sizeof(uint16_t)); }

    inline static CVectorI16T* cvector_create_i16() { return vector_create_generic(sizeof(int16_t)); }

    inline static CVectorU8T* cvector_create_u8() { return vector_create_generic(sizeof(uint8_t)); }

    inline static CVectorI8T* cvector_create_i8() { return vector_create_generic(sizeof(int8_t)); }

    inline static CVectorT* cvector_create_generic(size_t typeSize)
    {
        CVectorT* result = NULL;

        if (typeSize > 0) { result = (CVectorT*) CMALLOC(sizeof(cvectorT)); }
        if (NULL == result) { LOG_ERROR("Can not allocate cvector!\n"); }
        else
        {
            result->length = 0;                         // set length to 0
            result->capacity = CVECTOR_INITIAL_CAPACITY;// set capacity to cvector_INITIAL_CAPACITY
            result->elementSize = typeSize;             // set element size to stride
            int8_t* dataPtr = (int8_t*) CMALLOC(typeSize);
            if (NULL == dataPtr) { LOG_ERROR("Can not allocate cvector data buffer!\n"); }
            else { result->data = dataPtr; }
        }

        return result;
    }

    inline static void cvector_erase(CVectorT* vector, size_t index)
    {
        int8_t* resultPtr = NULL;
        if (NULL != vector->data)
        {
            void* dest = &(vector->data[index]);
            void* src = &(vector->data[index + vector->elementSize]);
            resultPtr = (int8_t*) CMEMCPY(dest, src, (vector->length - index));
            if (NULL == resultPtr) { LOG_ERROR("Can not copy cvector !\n"); }
        }
        if (NULL != resultPtr) { vector->length -= 1; }
    }

    inline static void cvector_erase_safe(CVectorT* vector, size_t index)
    {
        if (NULL == cvector) {}
        else if (index < vector->length) { vector_erase(darr, index); }
    }

    inline static void cvector_shrink_to_fit(CVectorT* vector)
    {
        if (vector->capacity > vector->length)
        {
            int8_t* resultPtr = NULL;
            if (NULL != vector->data)
            {
                resultPtr = (int8_t*) CREALLOC(vector->data, darr->length * darr->elementSize);
            }
            if (NULL == resultPtr) { LOG_ERROR("Can not reallocate cvector !\n"); }
            if (NULL != resultPtr)
            {
                vector->capacity = vector->length;
                vector->data = resultPtr;
            }
        }
    }

    inline static void cvector_reserve(CVectorT* vector, size_t newCapacity)
    {
        if (newCapacity > vector->capacity)
        {
            int8_t* resultPtr;

            if (NULL == vector->data) { resultPtr = (int8_t*) CMALLOC(newCapacity * vector->elementSize); }
            else { resultPtr = (int8_t*) CREALLOC(vector->data, newCapacity * vector->elementSize); }

            if (NULL == resultPtr) { LOG_ERROR("Can not allocate cvector !\n"); }
            if (NULL != resultPtr)
            {
                vector->data = resultPtr;
                vector->capacity = newCapacity;
            }
        }
    }

    inline static void* cvector_pop(CVectorT* vector)
    {
        void* valuePtr = NULL;
        valuePtr = cvector_back_ptr(vector);
        vector->length -= 1;
        return valuePtr;
    }

    inline static void* cvector_pop_safe(CVectorT* vector)
    {
        void* valuePtr = NULL;
        if (NULL == cvector) {}
        else if (vector->length > 0) { valuePtr = vector_pop(darr); }
        else { LOG_ERROR("Can not pop from cvector with size < 0!\n"); }
        return valuePtr;
    }
#ifdef __cplusplus
}
#endif
#endif// CVECTOR_HEADER
