#ifndef ZSERIO_OPTIONAL_HOLDER_H_INC
#define ZSERIO_OPTIONAL_HOLDER_H_INC

#include <cstddef>
#include <type_traits>

#include "zserio/CppRuntimeException.h"
#include "zserio/Types.h"

namespace zserio
{

/**
 * Helper type for specification of an unset optional holder.
 */
struct NullOptType
{
    /**
     * Explicit constructor from int.
     *
     * \see https://en.cppreference.com/w/cpp/utility/optional/nullopt_t
     */
    constexpr explicit NullOptType(int) {}
};

/**
 * Constant used to convenient specification of an unset optional holder.
 */
constexpr NullOptType NullOpt{int()};

namespace detail
{

/**
 * In place storage for optional holder.
 */
template <typename T>
class in_place_storage
{
public:
    /**
     * Constructor.
     */
    in_place_storage() {}

    /**
     * Destructor.
     */
    ~in_place_storage() = default;

    /** Copying is disabled. */
    /** \{ */
    in_place_storage(const in_place_storage&) = delete;
    in_place_storage& operator=(const in_place_storage&) = delete;
    /** \} */

    /**
     * Move constructor.
     *
     * \param other Other storage to move.
     */
    in_place_storage(in_place_storage&& other)
    {
        move(std::move(other));
    }

    /**
     * Move assignment operator.
     *
     * \param other Other storage to move.
     *
     * \return Reference to the current storage.
     */
    in_place_storage& operator=(in_place_storage&& other)
    {
        move(std::move(other));

        return *this;
    }

    /**
     * Gets pointer to the underlying in-place memory.
     *
     * \return Pointer to the storage.
     */
    void* getStorage()
    {
        return &m_inPlace;
    }

    /**
     * Gets pointer to the stored object.
     *
     * \return Pointer to the object.
     */
    T* getObject()
    {
        return reinterpret_cast<T*>(&m_inPlace);
    }

    /**
     * Gets pointer to the stored object.
     *
     * \return Const pointer to the object.
     */
    const T* getObject() const
    {
        return reinterpret_cast<const T*>(&m_inPlace);
    }

private:
    void move(in_place_storage&& other)
    {
        new (&m_inPlace) T(std::move(*other.getObject()));
        other.getObject()->~T(); // ensure that destructor of object in original storage is called
    }

    typename std::aligned_storage<sizeof(T), alignof(T)>::type m_inPlace;
};

/**
 * Optional holder implementation for Zserio which allows usage of both heap and in place storage.
 */
template <typename T, typename STORAGE>
class optional_holder
{
public:
    /**
     * Empty constructor which creates an unset holder.
     */
    constexpr optional_holder() noexcept
    {}

    /**
     * Constructor from zserio::NullOpt constant to create an unset holder.
     */
    constexpr optional_holder(NullOptType) noexcept
    {}

    /**
     * Constructor from a given value.
     *
     * \param value Value to store in the holder.
     */
    optional_holder(const T& value)
    {
        new (getStorage()) T(value);
        m_hasValue = true;
    }

    /**
     * Constructor from a given value passed by rvalue reference.
     *
     * \param value Value to store in the holder.
     */
    optional_holder(T&& value)
    {
        new (getStorage()) T(std::move(value));
        m_hasValue = true;
    }

    /**
     * Copy constructor.
     *
     * \param other Other holder to copy.
     */
    optional_holder(const optional_holder& other)
    {
        if (other.hasValue())
        {
            new (getStorage()) T(*other.m_storage.getObject());
            m_hasValue = true;
        }
    }

    /**
     * Move constructor.
     *
     * \param other Other holder to move.
     */
    optional_holder(optional_holder&& other)
    {
        if (other.hasValue())
        {
            other.m_hasValue = false;
            m_hasValue = true;
        }
    }

    /**
     * Destructor.
     */
    ~optional_holder()
    {
        reset();
    }

    /**
     * Assignment operator.
     *
     * \param other Other holder to copy-assign.
     *
     * \return Reference to the current holder.
     */
    optional_holder& operator=(const optional_holder& other)
    {
        if (this != &other)
        {
            reset();
            if (other.hasValue())
            {
                new (getStorage()) T(*other.m_storage.getObject());
                m_hasValue = true;
            }
        }

        return *this;
    }

    /**
     * Move assignment operator.
     *
     * \param other Other holder to move-assign.
     *
     * \return Reference to the current holder.
     */
    optional_holder& operator=(optional_holder&& other)
    {
        if (this != &other)
        {
            reset();
            if (other.hasValue())
            {
                m_storage = std::move(other.m_storage);
                other.m_hasValue = false;
                m_hasValue = true;
            }
        }

        return *this;
    }

    /**
     * Assignment operator from value.
     *
     * \param value Value to assign.
     *
     * \return Reference to the current holder.
     */
    optional_holder& operator=(const T& value)
    {
        set(value);

        return *this;
    }

    /**
     * Assignment operator from rvalue reference to value.
     *
     * \param value Value to move-assign.
     *
     * \return Reference to the current holder.
     */
    optional_holder& operator=(T&& value)
    {
        set(std::move(value));

        return *this;
    }

    /**
     * Operator equality.
     *
     * \param other Other holder to compare.
     *
     * \return True when the other holder has same value as this. False otherwise.
     */
    bool operator==(const optional_holder& other) const
    {
        if (this != &other)
        {
            if (hasValue() && other.hasValue())
                return *(m_storage.getObject()) == *(other.m_storage.getObject());

            return (!hasValue() && !other.hasValue());
        }

        return true;
    }

    /**
     * Bool operator.
     *
     * Evaluates to true when this holder has assigned any value.
     */
    explicit operator bool() const noexcept
    {
        return hasValue();
    }

    /**
     * Dereference operator *.
     *
     * \return Const reference to the assigned value.
     *
     * \throw CppRuntimeException when the holder is unset.
     */
    const T& operator*() const
    {
        return *get();
    }

    /**
     * Dereference operator *.
     *
     * \return Reference to the assigned value.
     *
     * \throw CppRuntimeException when the holder is unset.
     */
    T& operator*()
    {
        return *get();
    }

    /**
     * Dereference operator ->.
     *
     * \return Const reference to the assigned value.
     *
     * \throw CppRuntimeException when the holder is unset.
     */
    const T* operator->() const
    {
        return get();
    }

    /**
     * Dereference operator ->.
     *
     * \return Reference to the assigned value.
     *
     * \throw CppRuntimeException when the holder is unset.
     */
    T* operator->()
    {
        return get();
    }

    /**
     * Resets the current holder (switch to the unset state).
     */
    void reset() noexcept
    {
        if (hasValue())
        {
            m_storage.getObject()->~T();
            m_hasValue = false;
        }
    }

    /**
     * Gets held value.
     *
     * \return Const reference to the assigned value.
     *
     * \throw CppRuntimeException when the holder is unset.
     */
    const T& value() const
    {
        return *get();
    }

    /**
     * Gets held value.
     *
     * \return Reference to the assigned value.
     *
     * \throw CppRuntimeException when the holder is unset.
     */
    T& value()
    {
        return *get();
    }

    /**
     * Gets whether the holder has any value.
     *
     * \return True when this holder has assigned any value. False otherwise.
     */
    bool hasValue() const noexcept
    {
        return m_hasValue;
    }

private:
    template <typename U = T>
    void set(U&& value)
    {
        reset();
        new (getStorage()) T(std::forward<U>(value));
        m_hasValue = true;
    }

    T* get()
    {
        checkHasValue();
        return m_storage.getObject();
    }

    const T* get() const
    {
        checkHasValue();
        return m_storage.getObject();
    }

    void* getStorage()
    {
        return m_storage.getStorage();
    }

    void checkHasValue() const
    {
        if (!hasValue())
            throw CppRuntimeException("Trying to access value of non-present optional field!");
    }

    STORAGE m_storage;
    bool    m_hasValue = false;
};

} // namespace detail

// Be aware that if OptionalHolder is defined by typename, C++ compiler will have problem with template
// function overload, see HashCodeUtil.h (overloads for objects and for OptionalHolder).
/**
 * Optional holder which uses in place storage.
 */
template <typename T>
using OptionalHolder = detail::optional_holder<T, detail::in_place_storage<T>>;

} // namespace zserio

#endif // ifndef ZSERIO_OPTIONAL_HOLDER_H_INC
