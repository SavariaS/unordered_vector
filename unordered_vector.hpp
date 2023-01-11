#ifndef UNORDERED_VECTOR_HPP
#define UNORDERED_VECTOR_HPP

#include <cstddef> // std::size_t, std::ptrdiff_t
#include <memory>  // std::allocator
#include <algorithm> // std::copy
#include <stdexcept> // std::length_error, std::out_of_range
#include <limits> // std::numeric_limits
#include <string> // std::to_string

namespace xcontainer
{
    template<typename T, class Allocator = std::allocator<T>> 
    class unordered_vector
    {
        public:
            // Type definitions
            using value_type = T;
            using allocator_type = Allocator;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;
            using reference = value_type&;
            using const_reference = const value_type&;
            using pointer = typename std::allocator_traits<Allocator>::pointer;
            using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
            using iterator = value_type*;
            using const_iterator = const value_type*;
            using reverse_iterator = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;

            // Constructors
            constexpr unordered_vector() noexcept(noexcept(Allocator()))
            {
                m_allocator = Allocator();
                m_data = nullptr;
                m_size = 0;
                m_capacity = 0;
            }

            constexpr explicit unordered_vector(const Allocator& alloc) noexcept
            {
                m_allocator = alloc;
                m_data = nullptr;
                m_size = 0;
                m_capacity = 0;
            }

            constexpr unordered_vector(size_type count, const T& value, const Allocator& alloc = Allocator())
            {
                m_allocator = alloc;
                m_size = count;

                allocate(m_size);
                for(iterator itr = begin(); itr != end(); ++itr)
                {
                    *itr = value_type(value);
                }
            }

            constexpr explicit unordered_vector(size_type count, const Allocator& alloc = Allocator())
            {
                m_allocator = alloc;
                m_size = count;

                allocate(m_size);
            }

            template<class InputItr>
            constexpr unordered_vector(InputItr first, InputItr last, const Allocator& alloc = Allocator())
            {
                m_allocator = alloc;
                m_size = 0;

                for(InputItr itr = first; itr != last; ++itr)
                {
                    ++m_size;
                }

                allocate(m_size);
                std::copy(first, last, begin());
            }

            constexpr unordered_vector(const unordered_vector& other)
            {
                m_allocator = other.m_allocator;
                m_size = other.m_size;
                m_capacity = other.m_capacity;

                m_data = m_allocator.allocate(m_capacity);
                std::copy(other.begin(), other.end(), begin());
            }

            constexpr unordered_vector(const unordered_vector& other, const Allocator& alloc)
            {
                m_allocator = alloc;
                m_size = other.m_size;
                m_capacity = other.m_capacity;

                m_data = m_allocator.allocate(m_capacity);
                std::copy(other.begin(), other.end(), begin());
            }

            constexpr unordered_vector(unordered_vector&& other) noexcept
            {
                m_allocator = other.m_allocator;
                m_size = other.m_size;
                m_capacity = other.m_capacity;
                m_data = other.m_data;

                other.m_data = nullptr;
                other.m_size = 0;
                other.m_capacity = 0;
            }

            constexpr unordered_vector(unordered_vector&& other, const Allocator& alloc)
            {
                m_allocator = alloc;
                m_size = other.m_size;
                m_capacity = other.m_capacity;

                if(m_allocator == other.m_allocator)
                {
                    m_data = other.m_data;

                    other.m_data = nullptr;
                    other.m_size = 0;
                    other.m_capacity = 0;
                }
                else
                {
                    m_data = alloc.allocate(m_size);
                    std::copy(other.begin(), other.end(), begin());
                }
            }

            constexpr unordered_vector(std::initializer_list<T> init, const Allocator& alloc = Allocator())
            {
                m_allocator = alloc;
                m_size = init.size();
                allocate(m_size);

                std::copy(init.begin(), init.end(), begin());
            }

            ~unordered_vector()
            {
                if(m_data != nullptr)
                {
                    m_allocator.deallocate(m_data, m_capacity);
                }
            }

            // Element access
            constexpr reference at(size_type pos)
            {
                if(pos < m_size)
                {
                    return m_data[pos];
                }
                else
                {
                    throw std::out_of_range("pos (which is " + std::to_string(pos) + ") >= this->size() (which is " + std::to_string(m_size) + ")");
                }
            }

            constexpr const_reference at(size_type pos) const
            {
                if(pos < m_size)
                {
                    return m_data[pos];
                }
                else
                {
                    throw std::out_of_range("pos (which is " + std::to_string(pos) + ") >= this->size() (which is " + std::to_string(m_size) + ")");
                }
            }

            constexpr reference operator[](size_type pos)
            {
                return m_data[pos];
            }

            constexpr const_reference operator[](size_type pos) const
            {
                return m_data[pos];
            }

            constexpr reference front()
            {
                return m_data[0];
            }

            constexpr const_reference front() const
            {
                return m_data[0];
            }

            constexpr reference back()
            {
                return m_data[m_size - 1];
            }

            constexpr const_reference back() const
            {
                return m_data[m_size - 1];
            }

            constexpr T* data() noexcept
            {
                return m_data;
            }

            constexpr const T* data() const noexcept
            {
                return m_data;
            }

            // Iterators
            constexpr iterator begin() noexcept
            {
                return iterator(m_data);
            }

            const_iterator begin() const noexcept
            {
                return const_iterator(m_data);
            }

            const_iterator cbegin() const noexcept
            {
                return const_iterator(m_data);
            }

            constexpr iterator end() noexcept
            {
                return iterator(m_data + m_size);
            }

            constexpr const_iterator end() const noexcept
            {
                return const_iterator(m_data + m_size);
            }

            constexpr const_iterator cend() const noexcept
            {
                return const_iterator(m_data + m_size);
            }

            constexpr reverse_iterator rbegin() noexcept
            {
                return reverse_iterator(end());
            }

            constexpr const_reverse_iterator rbegin() const noexcept
            {
                return const_reverse_iterator(cend());
            }

            constexpr const_reverse_iterator crbegin() const noexcept
            {
                return const_reverse_iterator(cend());
            }

            constexpr reverse_iterator rend() noexcept
            {
                return reverse_iterator(begin());
            }

            constexpr const_reverse_iterator rend() const noexcept
            {
                return reverse_iterator(cbegin());
            }

            constexpr const_reverse_iterator crend() const noexcept
            {
                return reverse_iterator(cbegin());
            }

            // Capacity
            [[nodiscard]] constexpr bool empty() const noexcept
            {
                return (m_size == 0) ? true : false;
            }

            constexpr size_type size() const noexcept
            {
                return m_size;
            }

            constexpr size_type max_size() const noexcept
            {
                return std::numeric_limits<size_type>::max();
            }

            constexpr void reserve(size_type new_cap)
            {
                if(new_cap > m_capacity)
                {
                    // Allocate the new memory
                    value_type* new_data = m_allocator.allocate(new_cap);

                    // Copy and deallocate the existing vector, if it exists
                    if(m_data != nullptr)
                    {
                        std::copy(begin(), end(), new_data);
                        m_allocator.deallocate(m_data, m_capacity);
                    }

                    // Use the new memory
                    m_data = new_data;
                    m_capacity = new_cap;
                }
                else
                {
                    throw std::length_error("New capacity inferior to current capacity");
                }
            }

            constexpr size_type capacity() const noexcept
            {
                return m_capacity;
            }

            constexpr void shrink_to_fit()
            {
                if(m_size == 0)
                {
                    m_allocator.deallocate(m_data, m_capacity);

                    m_data = nullptr;
                    m_capacity = 0;
                }
                else
                {
                    value_type* new_data = m_allocator.allocate(m_size);

                    std::copy(begin(), end(), new_data);
                    m_allocator.deallocate(m_data, m_capacity);

                    m_data = new_data;
                    m_capacity = m_size;
                }
            }

            // Modifiers
            constexpr void clear() noexcept
            {
                // Destroy all elements
                for(iterator itr = begin(); itr != end(); ++itr)
                {
                    *itr = value_type();
                }

                m_size = 0;
            }

            constexpr iterator insert(const_iterator pos, const T& value)
            {
                difference_type index = pos - begin();

                push_back(std::move(m_data[index]));
                m_data[index] = value_type(value);

                return begin() + index;
            }

            constexpr iterator insert(const_iterator pos, T&& value)
            {
                difference_type index = pos - begin();

                push_back(std::move(m_data[index]));
                m_data[index] = value_type(value);

                return begin() + index;
            }

            constexpr iterator insert(const_iterator pos, size_type count, const T& value)
            {
                difference_type index = pos - begin();

                // Allocate memory
                if(m_size + count > m_capacity)
                {
                    allocate(m_size + count);
                }

                // Insert values
                for(size_type i = 0; i < count; ++i)
                {
                    push_back(std::move(m_data[index + i]));
                    m_data[index + i] = value_type(value);
                }

                return begin() + index;
            }

            template<class InputItr>
            constexpr iterator insert(const_iterator pos, InputItr first, InputItr last)
            {
                difference_type index = pos - begin();

                // Insert range
                size_type i = 0;
                for(InputItr itr = first; itr != last; ++itr)
                {
                    push_back(std::move(m_data[index + i]));
                    m_data[index + i] = value_type(*itr);

                    ++i;
                }

                return begin() + index;
            }

            constexpr iterator insert(const_iterator pos, std::initializer_list<T> ilist)
            {
                difference_type index = pos - begin();
                size_type count = ilist.size();

                // Allocate memory
                if(m_size + count > m_capacity)
                {
                    allocate(m_size + count);
                }

                // Insert list
                size_type i = 0;
                for(const T& value : ilist)
                {
                    push_back(std::move(m_data[index + i]));
                    m_data[index + i] = value_type(value);

                    ++i;
                }

                return begin() + index;
            }

            template< class... Args >
            constexpr iterator emplace(const_iterator pos, Args&&... args)
            {
                return insert(pos, value_type(std::forward<Args>(args)...));
            }

            constexpr iterator erase(const_iterator pos)
            {
                difference_type index = pos - cbegin();

                m_data[index] = std::move(back());
                pop_back();

                return begin() + index;

            }

            constexpr iterator erase(const_iterator first, const_iterator last)
            {
                difference_type index = 0;

                for(const_iterator itr = last - 1; itr != first - 1; --itr)
                {
                    index = itr - cbegin();

                    m_data[index] = std::move(back());
                    pop_back();
                }

                return begin() + index;
            }

            constexpr void push_back(const T& value)
            {
                // Increase the size
                ++m_size;
                if(m_size > m_capacity)
                {
                    allocate(m_size);
                }

                m_data[m_size - 1] = value_type(value);
            }

            constexpr void push_back(T&& value)
            {
                // Increase the size
                ++m_size;
                if(m_size > m_capacity)
                {
                    allocate(m_size);
                }

                m_data[m_size - 1] = value_type(value);
            }

            template<class... Args>
            constexpr reference emplace_back(Args&&... args)
            {
                // Increase the size
                ++m_size;
                if(m_size > m_capacity)
                {
                    allocate(m_size);
                }

                m_data[m_size - 1] = value_type(std::forward<Args>(args)...);
            }

            constexpr void pop_back()
            {
                m_data[m_size - 1] = value_type();
                --m_size;
            }

            constexpr void resize(size_type count)
            {
                if(m_size > count)
                {
                    while(m_size != count)
                    {
                        pop_back();
                    }
                }
                else if(m_size < count)
                {
                    if(m_size + count > m_capacity)
                    {
                        allocate(m_size + count);
                    }

                    while(m_size != count)
                    {
                        push_back(value_type());
                    }
                }
            }

            constexpr void resize(size_type count, const value_type& value)
            {
                if(m_size > count)
                {
                    while(m_size != count)
                    {
                        pop_back();
                    }
                }
                else if(m_size < count)
                {
                    if(m_size + count > m_capacity)
                    {
                        allocate(m_size + count);
                    }

                    while(m_size != count)
                    {
                        push_back(value);
                    }
                }
            }

            constexpr void swap(unordered_vector& other) noexcept(std::allocator_traits<Allocator>::propagate_on_container_swap::value || std::allocator_traits<Allocator>::is_always_equal::value)
            {
                value_type* data = m_data;
                size_type size = m_size;
                size_type capacity = m_capacity;
                allocator_type allocator = m_allocator;

                m_data = other.m_data;
                m_size = other.m_size;
                m_capacity = other.m_capacity;
                m_allocator = other.m_allocator;

                other.m_data = data;
                other.m_size = size;
                other.m_capacity = capacity;
                other.m_allocator = allocator;
            }

            // Other
            constexpr unordered_vector& operator=(const unordered_vector& other)
            {
                m_allocator = other.m_allocator;
                m_size = other.m_size;
                m_capacity = other.m_capacity;

                m_data = m_allocator.allocate(m_capacity);
                std::copy(other.begin(), other.end(), begin());

                return *this;
            }

            constexpr unordered_vector& operator=(unordered_vector&& other) noexcept(std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value || std::allocator_traits<Allocator>::is_always_equal::value)
            {
                m_allocator = other.m_allocator;
                m_size = other.m_size;
                m_capacity = other.m_capacity;
                m_data = other.m_data;

                other.m_data = nullptr;
                other.m_size = 0;
                other.m_capacity = 0;

                return *this;
            }

            constexpr unordered_vector& operator=(std::initializer_list<T> ilist)
            {
                m_allocator = Allocator();
                m_size = ilist.size();
                allocate(m_size);

                std::copy(ilist.begin(), ilist.end(), begin());

                return *this;
            }

            constexpr void assign(size_type count, const T& value)
            {
                clear();
                if(count > m_capacity)
                {
                    allocate(count);
                }

                for(iterator itr = begin(); itr != end; ++itr)
                {
                    *itr = value_type(value);
                }
            }

            template<class InputItr>
            constexpr void assign(InputItr first, InputItr last)
            {
                clear();

                for(InputItr itr = first; itr != last; ++itr)
                {
                    push_back(value_type(*itr));
                }
            }

            constexpr void assign(std::initializer_list<T> ilist)
            {
                clear();
                if(ilist.size() > m_capacity)
                {
                    allocate(ilist.size());
                }

                for(const T& value : ilist)
                {
                    push_back(value_type(value));
                }
            }

            constexpr allocator_type get_allocator() const noexcept
            {
                return m_allocator;
            }

        private:
            value_type* m_data = nullptr;
            size_type m_size = 0;
            size_type m_capacity = 0;
            allocator_type m_allocator = allocator_type();

            void allocate(size_type size)
            {
                if(size > m_capacity)
                {
                    // Get the nearest power of 2
                    size_type power = 1;
                    while(power < size)
                    {
                        power *= 2;
                    }
                    
                    // Allocate the memory
                    reserve(power);
                }
            }
    };
}

namespace std
{
    template<class T, class Alloc, class U>
    constexpr typename xcontainer::unordered_vector<T, Alloc>::size_type erase(xcontainer::unordered_vector<T, Alloc>& c, const U& value)
    {
        auto it = std::remove(c.begin(), c.end(), value);
        auto r = std::distance(it, c.end());
        c.erase(it, c.end());
        return r;
    }

    template<class T, class Alloc, class Pred>
    constexpr typename xcontainer::unordered_vector<T, Alloc>::size_type erase_if(xcontainer::unordered_vector<T, Alloc>& c, Pred pred)
    {
        auto it = std::remove_if(c.begin(), c.end(), pred);
        auto r = std::distance(it, c.end());
        c.erase(it, c.end());
        return r;
    }

    template<class T, class Alloc>
    constexpr void swap(xcontainer::unordered_vector<T, Alloc>& lhs, xcontainer::unordered_vector<T, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
    {
        lhs.swap(rhs);
    }
}

#endif