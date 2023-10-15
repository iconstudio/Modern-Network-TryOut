export module Net.Property:IProperty;
import Net.Constraints;
import <type_traits>;
import <functional>;

export namespace net
{
	template<typename T, typename Context, bool Custom, bool Copyable, bool Readonly>
	class IProperty;

	template<movable T, typename Context, bool Copyable, bool Readonly>
	class IProperty<T, Context, false, Copyable, Readonly>
	{
	public:
		constexpr IProperty()
			noexcept(nothrow_default_constructibles<T>) = default;
		virtual constexpr ~IProperty()
			noexcept(nothrow_destructibles<T>) = default;

		template<convertible_to<T> U>
		constexpr IProperty(U&& trans_value)
			noexcept(nothrow_constructible<T, U&&>) requires constructible_from<T, U&&>
			: myValue(std::forward<U>(trans_value))
		{}

		template<convertible_to<T> U, typename X2, bool S2, bool C2, bool R2>
		constexpr IProperty(const IProperty<U, X2, S2, C2, R2>& other)
			noexcept(nothrow_constructible<T, const U&>)
			requires Copyable and C2 and copy_constructibles<T, U> and constructible_from<T, const U&>
		: myValue(other.myValue)
		{}

		template<convertible_to<T> U, typename X2, bool S2, bool C2, bool R2>
		constexpr IProperty(IProperty<U, X2, S2, C2, R2>&& other)
			noexcept(nothrow_constructible<T, U&&>)
			requires move_constructibles<T, U> and constructible_from<T, U&&>
		: myValue(std::move(other.myValue))
		{}

		[[nodiscard]]
		constexpr operator T& () & noexcept
		{
			return myValue;
		}

		[[nodiscard]]
		constexpr operator const T& () const& noexcept
		{
			return myValue;
		}

		[[nodiscard]]
		constexpr operator T && () && noexcept
		{
			return std::move(myValue);
		}

		[[nodiscard]]
		constexpr operator const T && () const&& noexcept
		{
			return std::move(myValue);
		}

		template<convertible_to<T> U, typename X2, bool S2, bool C2, bool R2>
		constexpr IProperty& operator=(const IProperty<U, X2, S2, C2, R2>& other)
			noexcept(nothrow_assignable<const U&, T>)
			requires (not Readonly and C2 and copy_assignables<T, U> and constructible_from<T, const U&>)
		{
			myValue = other.myValue;
			return *this;
		}

		template<convertible_to<T> U, typename X2, bool S2, bool C2, bool R2>
		constexpr IProperty& operator=(IProperty<U, X2, S2, C2, R2>&& other)
			noexcept(nothrow_assignable<U&&, T>)
			requires (not Readonly and move_assignables<T, U> and constructible_from<T, U&&>)
		{
			myValue = std::move(other.myValue);
			return *this;
		}

	protected:
		T myValue;
	};

	template<movable T, bool Copyable, bool Readonly>
	class IProperty<T, void, true, Copyable, Readonly>
	{
	public:
		using functor_t = std::function<void(T&)>;

		constexpr IProperty()
			noexcept(nothrow_default_constructibles<T>) = default;
		virtual constexpr ~IProperty()
			noexcept(nothrow_destructibles<T>) = default;

		template<convertible_to<T> U, invocables<T&> Fn>
		constexpr IProperty(U&& trans_value, Fn&& setter)
			noexcept(nothrow_constructible<T, U&&> and nothrow_default_constructibles<T> and nothrow_constructible<functor_t, Fn&&>)
			requires constructible_from<T, U&&> and constructible_from<functor_t, Fn&&>
		: myValue(std::forward<U>(trans_value)), mySetter(std::forward<Fn>(setter))
		{}

		template<bool C2, bool R2>
		constexpr IProperty(const IProperty<T, void, true, C2, R2>& other)
			noexcept(nothrow_copy_constructibles<T>)
			requires Copyable and C2 and copy_constructible<T>
		: myValue(other.myValue), mySetter(other.mySetter)
		{}

		template<bool C2, bool R2>
		constexpr IProperty(IProperty<T, void, true, C2, R2>&& other)
			noexcept(nothrow_move_constructibles<T>)
			requires move_constructibles<T>
		: myValue(other.myValue), mySetter(std::move(other.mySetter))
		{}

		template<convertible_to<T> U, typename X2, bool S2, bool C2, bool R2, invocables<T&> Fn>
		constexpr IProperty(const IProperty<U, X2, S2, C2, R2>& other, Fn&& setter)
			noexcept(nothrow_constructible<T, const U&> and nothrow_constructible<functor_t, Fn&&>)
			requires Copyable and C2 and copy_constructibles<T, U> and constructible_from<T, const U&> and constructible_from<functor_t, Fn&&>
		: myValue(other.myValue), mySetter(std::forward<Fn>(setter))
		{}

		template<convertible_to<T> U, typename X2, bool S2, bool C2, bool R2, invocables<T&> Fn>
		constexpr IProperty(IProperty<U, X2, S2, C2, R2>&& other, Fn&& setter)
			noexcept(nothrow_constructible<T, U&&> and nothrow_constructible<functor_t, Fn&&>)
			requires move_constructibles<T, U> and constructible_from<T, U&&> and constructible_from<functor_t, Fn&&>
		: myValue(std::move(other.myValue)), mySetter(std::forward<Fn>(setter))
		{}

		[[nodiscard]]
		constexpr operator T& () & noexcept
		{
			return myValue;
		}

		[[nodiscard]]
		constexpr operator const T& () const& noexcept
		{
			return myValue;
		}

		[[nodiscard]]
		constexpr operator T && () && noexcept
		{
			return std::move(myValue);
		}

		[[nodiscard]]
		constexpr operator const T && () const&& noexcept
		{
			return std::move(myValue);
		}

		template<convertible_to<T> U, typename X2, bool S2, bool C2, bool R2>
		constexpr IProperty& operator=(const IProperty<U, X2, S2, C2, R2>& other)
			noexcept(nothrow_assignable<const U&, T>)
			requires not Readonly and C2 and copy_assignables<T, U> and constructible_from<T, const U&>
		{
			myValue = other.myValue;
			return *this;
		}

		template<convertible_to<T> U, typename X2, bool S2, bool C2, bool R2>
		constexpr IProperty& operator=(IProperty<U, X2, S2, C2, R2>&& other)
			noexcept(nothrow_assignable<U&&, T>)
			requires not Readonly and move_assignables<T, U> and constructible_from<T, U&&>
		{
			myValue = std::move(other.myValue);
			return *this;
		}

	protected:
		T myValue;
		functor_t mySetter;
	};

	template<movable T, typename Context, bool Copyable, bool Readonly>
	class IProperty<T, Context, true, Copyable, Readonly>
	{
	public:
		using functor_t = std::function<void(Context&, T&)>;

		constexpr IProperty()
			noexcept(nothrow_default_constructibles<T>) = default;
		virtual constexpr ~IProperty()
			noexcept(nothrow_destructibles<T>) = default;

		template<convertible_to<T> U, invocables<Context&, T&> Fn>
		constexpr IProperty(Context* const& context, U&& trans_value, Fn&& setter)
			noexcept(nothrow_constructible<T, U&&> and nothrow_default_constructibles<T> and nothrow_constructible<functor_t, Fn&&>) requires constructible_from<T, U&&> and constructible_from<functor_t, Fn&&>
			: myContext(context)
			, myValue(std::forward<U>(trans_value))
			, mySetter(std::forward<Fn>(setter))
		{}

		template<bool S2, bool C2, bool R2>
			requires Copyable and C2 and copy_constructible<T>
		constexpr IProperty(const IProperty<T, Context, true, C2, R2>& other)
			noexcept(nothrow_copy_constructibles<T>)
			: myContext(other.myContext)
			, myValue(other.myValue)
			, mySetter(other.mySetter)
		{}

		template<bool S2, bool C2, bool R2>
			requires move_constructibles<T>
		constexpr IProperty(IProperty<T, Context, true, C2, R2>&& other)
			noexcept(nothrow_move_constructibles<T>)
			: myContext(other.myContext)
			, myValue(other.myValue)
			, mySetter(std::move(other.mySetter))
		{}

		template<convertible_to<T> U, typename X2, bool S2, bool C2, bool R2, invocables<T&> Fn>
			requires Copyable and C2 and copy_constructibles<T, U> and constructible_from<T, const U&> and constructible_from<functor_t, Fn&&>
		constexpr IProperty(Context* const& context, const IProperty<U, X2, S2, C2, R2>& other, Fn&& setter)
			noexcept(nothrow_constructible<T, const U&> and nothrow_constructible<functor_t, Fn&&>)
			: myContext(context)
			, myValue(other.myValue)
			, mySetter(std::forward<Fn>(setter))
		{}

		template<convertible_to<T> U, typename X2, bool S2, bool C2, bool R2, invocables<T&> Fn>
			requires move_constructibles<T, U> and constructible_from<T, U&&> and constructible_from<functor_t, Fn&&>
		constexpr IProperty(Context* const& context, IProperty<U, X2, S2, C2, R2>&& other, Fn&& setter)
			noexcept(nothrow_constructible<T, U&&> and nothrow_constructible<functor_t, Fn&&>)
			: myContext(context)
			, myValue(std::move(other.myValue))
			, mySetter(std::forward<Fn>(setter))
		{}

		[[nodiscard]]
		constexpr operator T& () & noexcept
		{
			return myValue;
		}

		[[nodiscard]]
		constexpr operator const T& () const& noexcept
		{
			return myValue;
		}

		[[nodiscard]]
		constexpr operator T && () && noexcept
		{
			return std::move(myValue);
		}

		[[nodiscard]]
		constexpr operator const T && () const&& noexcept
		{
			return std::move(myValue);
		}

		template<convertible_to<T> U, typename X2, bool S2, bool C2, bool R2>
		constexpr IProperty& operator=(const IProperty<U, X2, S2, C2, R2>& other)
			noexcept(nothrow_assignable<const U&, T>)
			requires not Readonly and C2 and copy_assignables<T, U> and constructible_from<T, const U&>
		{
			myValue = other.myValue;
			return *this;
		}

		template<convertible_to<T> U, typename X2, bool S2, bool C2, bool R2>
		constexpr IProperty& operator=(IProperty<U, X2, S2, C2, R2>&& other)
			noexcept(nothrow_assignable<U&&, T>)
			requires not Readonly and move_assignables<T, U> and constructible_from<T, U&&>
		{
			myValue = std::move(other.myValue);
			return *this;
		}

	protected:
		Context* myContext;
		T myValue;
		functor_t mySetter;
	};
}
