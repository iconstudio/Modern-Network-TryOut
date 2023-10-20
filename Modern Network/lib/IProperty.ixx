export module Net.Property:IProperty;
import Net.Constraints;

export namespace net
{
	template<typename T, typename Context, bool Custom, bool Copyable, bool Readonly>
	class IProperty;

	template<movable T, typename Context, bool Copyable, bool Readonly>
	class IProperty<T, Context, false, Copyable, Readonly> final
	{
	public:
		constexpr IProperty()
			noexcept(nothrow_default_constructibles<T>)
			requires default_initializable<T> = default;
		constexpr ~IProperty()
			noexcept(nothrow_destructibles<T>) = default;

		template<convertible_to<T> U>
			requires constructible_from<T, U&&>
		constexpr IProperty(U&& trans_value)
			noexcept(nothrow_constructible<T, U&&>)
			: myValue(static_cast<U&&>(trans_value))
		{}

		template<convertible_to<T> U, typename X2, bool S2, bool C2, bool R2>
			requires Copyable and C2 and copy_constructibles<T, U> and constructible_from<T, const U&>
		constexpr IProperty(const IProperty<U, X2, S2, C2, R2>& other)
			noexcept(nothrow_constructible<T, const U&>)
			: myValue(other.myValue)
		{}

		template<convertible_to<T> U, typename X2, bool S2, bool C2, bool R2>
			requires move_constructibles<T, U> and constructible_from<T, U&&>
		constexpr IProperty(IProperty<U, X2, S2, C2, R2>&& other)
			noexcept(nothrow_constructible<T, U&&>)
			: myValue(static_cast<U&&>(other.myValue))
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
			return static_cast<T&&>(myValue);
		}

		[[nodiscard]]
		constexpr operator const T && () const&& noexcept
		{
			return static_cast<const T&&>(myValue);
		}

		template<convertible_to<T> U, typename X2, bool S2, bool C2, bool R2>
			requires not Readonly and C2 and copy_assignables<T, U> and constructible_from<T, const U&>
		constexpr IProperty& operator=(const IProperty<U, X2, S2, C2, R2>& other)
			noexcept(nothrow_assignable<const U&, T>)
		{
			myValue = other.myValue;
			return *this;
		}

		template<convertible_to<T> U, typename X2, bool S2, bool C2, bool R2>
			requires not Readonly and move_assignables<T, U> and constructible_from<T, U&&>
		constexpr IProperty& operator=(IProperty<U, X2, S2, C2, R2>&& other)
			noexcept(nothrow_assignable<U&&, T>)
		{
			myValue = static_cast<T>(other.myValue);
			return *this;
		}

	protected:
		T myValue;
	};

	template<movable T, bool Copyable, bool Readonly>
	class IProperty<T, void, true, Copyable, Readonly> final
	{
	public:
		using functor_t = void(*)(T&);

		constexpr IProperty()
			noexcept(nothrow_default_constructibles<T>)
			requires default_initializable<T> = default;
		constexpr ~IProperty()
			noexcept(nothrow_destructibles<T>) = default;

		template<convertible_to<T> U, invocables<T&> Fn>
		constexpr IProperty(U&& trans_value, Fn&& setter)
			noexcept(nothrow_constructible<T, U&&> and nothrow_default_constructibles<T> and nothrow_constructible<functor_t, Fn&&>)
			requires constructible_from<T, U&&> and constructible_from<functor_t, Fn&&>
		: myValue(static_cast<U&&>(trans_value)), mySetter(static_cast<functor_t>(setter))
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
		: myValue(other.myValue), mySetter(static_cast<functor_t&&>(other.mySetter))
		{}

		template<convertible_to<T> U, typename X2, bool S2, bool C2, bool R2, invocables<T&> Fn>
			requires Copyable and C2 and copy_constructibles<T, U> and constructible_from<T, const U&> and constructible_from<functor_t, Fn&&>
		constexpr IProperty(const IProperty<U, X2, S2, C2, R2>& other, Fn&& setter)
			noexcept(nothrow_constructible<T, const U&> and nothrow_constructible<functor_t, Fn&&>)
			: myValue(other.myValue), mySetter(setter)
		{}

		template<convertible_to<T> U, typename X2, bool S2, bool C2, bool R2, invocables<T&> Fn>
			requires move_constructibles<T, U> and constructible_from<T, U&&> and constructible_from<functor_t, Fn&&>
		constexpr IProperty(IProperty<U, X2, S2, C2, R2>&& other, Fn&& setter)
			noexcept(nothrow_constructible<T, U&&> and nothrow_constructible<functor_t, Fn&&>)
			: myValue(static_cast<U&&>(other.myValue)), mySetter(static_cast<functor_t&&>(setter))
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
			return static_cast<T&&>(myValue);
		}

		[[nodiscard]]
		constexpr operator const T && () const&& noexcept
		{
			return static_cast<const T&&>(myValue);
		}

		template<convertible_to<T> U>
			requires not Readonly and assignable_from<T&, U&&>
		constexpr IProperty& operator=(U&& value)
		{
			myValue = static_cast<U&&>(value);
			mySetter(myValue);

			return *this;
		}

		template<convertible_to<T> U, typename X2, bool S2, bool C2, bool R2>
			requires not Readonly and C2 and copy_assignables<T, U> and constructible_from<T, const U&>
		constexpr IProperty& operator=(const IProperty<U, X2, S2, C2, R2>& other)
			noexcept(nothrow_assignable<const U&, T>)
		{
			myValue = other.myValue;
			mySetter(myValue);

			return *this;
		}

		template<convertible_to<T> U, typename X2, bool S2, bool C2, bool R2>
			requires not Readonly and move_assignables<T, U> and constructible_from<T, U&&>
		constexpr IProperty& operator=(IProperty<U, X2, S2, C2, R2>&& other)
			noexcept(nothrow_assignable<U&&, T>)
		{
			myValue = static_cast<U&&>(other.myValue);
			mySetter(myValue);

			return *this;
		}

	protected:
		T myValue;
		functor_t mySetter;
	};

	template<movable T, typename Context, bool Copyable, bool Readonly>
	class IProperty<T, Context, true, Copyable, Readonly> final
	{
	public:
		using functor_t = void(*)(Context&, T&);

		constexpr IProperty()
			noexcept(nothrow_default_constructibles<T>)
			requires default_initializable<T> = default;
		constexpr ~IProperty()
			noexcept(nothrow_destructibles<T>) = default;

		template<convertible_to<T> U, invocables<Context&, T&> Fn>
			requires constructible_from<T, U&&> and constructible_from<functor_t, Fn&&>
		constexpr IProperty(Context* const& context, U&& trans_value, Fn&& setter)
			noexcept(nothrow_constructible<T, U&&> and nothrow_default_constructibles<T> and nothrow_constructible<functor_t, Fn&&>)
			: myContext(context)
			, myValue(static_cast<U&&>(trans_value))
			, mySetter(static_cast<functor_t>(setter))
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
			, mySetter(static_cast<functor_t&&>(other.mySetter))
		{}

		template<convertible_to<T> U, typename X2, bool S2, bool C2, bool R2, invocables<Context&, T&> Fn>
			requires Copyable and C2 and copy_constructibles<T, U> and constructible_from<T, const U&> and constructible_from<functor_t, Fn&&>
		constexpr IProperty(Context* const& context, const IProperty<U, X2, S2, C2, R2>& other, Fn&& setter)
			noexcept(nothrow_constructible<T, const U&> and nothrow_constructible<functor_t, Fn&&>)
			: myContext(context)
			, myValue(other.myValue)
			, mySetter(setter)
		{}

		template<convertible_to<T> U, typename X2, bool S2, bool C2, bool R2, invocables<Context&, T&> Fn>
			requires move_constructibles<T, U> and constructible_from<T, U&&> and constructible_from<functor_t, Fn&&>
		constexpr IProperty(Context* const& context, IProperty<U, X2, S2, C2, R2>&& other, Fn&& setter)
			noexcept(nothrow_constructible<T, U&&> and nothrow_constructible<functor_t, Fn&&>)
			: myContext(context)
			, myValue(static_cast<U&&>(other.myValue))
			, mySetter(static_cast<functor_t&&>(setter))
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
			return static_cast<T&&>(myValue);
		}

		[[nodiscard]]
		constexpr operator const T && () const&& noexcept
		{
			return static_cast<const T&&>(myValue);
		}

		template<convertible_to<T> U>
			requires not Readonly and assignable_from<T&, U&&>
		constexpr IProperty& operator=(U&& value)
		{
			myValue = static_cast<U&&>(value);
			mySetter(*myContext, myValue);

			return *this;
		}

		template<convertible_to<T> U, typename X2, bool S2, bool C2, bool R2>
			requires not Readonly and C2 and copy_assignables<T, U> and constructible_from<T, const U&>
		constexpr IProperty& operator=(const IProperty<U, X2, S2, C2, R2>& other)
		{
			myValue = other.myValue;
			mySetter(*myContext, myValue);

			return *this;
		}

		template<convertible_to<T> U, typename X2, bool S2, bool C2, bool R2>
			requires not Readonly and move_assignables<T, U> and constructible_from<T, U&&>
		constexpr IProperty& operator=(IProperty<U, X2, S2, C2, R2>&& other)
		{
			myValue = static_cast<U&&>(other.myValue);
			mySetter(*myContext, myValue);

			return *this;
		}

	protected:
		Context* const myContext;
		T myValue;
		functor_t mySetter;
	};
}
