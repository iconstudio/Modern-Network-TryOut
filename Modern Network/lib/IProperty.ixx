export module Net.Property:IProperty;
export import Net.Constraints;
export import <type_traits>;
export import <functional>;

export namespace net
{
	template<typename T, typename Context, typename Setter, bool Copyable, bool Readonly>
	class IProperty;

	template<movable T, notvoids Context, bool Copyable, bool Readonly>
	class IProperty<T, Context, void, Copyable, Readonly>
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

		template<convertible_to<T> U, typename X2, typename S2, bool C2, bool R2>
		constexpr IProperty(const IProperty<U, X2, S2, C2, R2>& other)
			noexcept(nothrow_constructible<T, const U&>)
			requires Copyable and C2 and copy_constructibles<T, U> and constructible_from<T, const U&>
		: myValue(other.myValue)
		{}

		template<convertible_to<T> U, typename X2, typename S2, bool C2, bool R2>
		constexpr IProperty(IProperty<U, X2, S2, C2, R2>&& other)
			noexcept(nothrow_constructible<T, U&&>)
			requires move_constructibles<T, U> and constructible_from<T, U&&>
		: myValue(std::move(other.myValue))
		{}

		[[nodiscard]]
		constexpr T& value() & noexcept
		{
			return myValue;
		}

		[[nodiscard]]
		constexpr const T& value() const& noexcept
		{
			return myValue;
		}

		[[nodiscard]]
		constexpr T&& value() && noexcept
		{
			return std::move(myValue);
		}

		[[nodiscard]]
		constexpr const T&& value() const&& noexcept
		{
			return std::move(myValue);
		}

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

		template<convertible_to<T> U, typename X2, typename S2, bool C2, bool R2>
		constexpr IProperty& operator=(const IProperty<U, X2, S2, C2, R2>& other)
			noexcept(nothrow_assignable<const U&, T>)
			requires !Readonly and C2 and copy_assignables<T, U> and constructible_from<T, const U&>
		{
			myValue = other.myValue;
			return *this;
		}

		template<convertible_to<T> U, typename X2, typename S2, bool C2, bool R2>
		constexpr IProperty& operator=(IProperty<U, X2, S2, C2, R2>&& other)
			noexcept(nothrow_assignable<U&&, T>)
			requires !Readonly and move_assignables<T, U> and constructible_from<T, U&&>
		{
			myValue = std::move(other.myValue);
			return *this;
		}

	protected:
		T myValue;
	};

	template<movable T, notvoids Context, invocables<T&> Setter, bool Copyable, bool Readonly>
	class IProperty<T, Context, Setter, Copyable, Readonly>
	{
	public:
		constexpr IProperty()
			noexcept(nothrow_default_constructibles<T>) = default;
		virtual constexpr ~IProperty()
			noexcept(nothrow_destructibles<T>) = default;

		template<convertible_to<T> U>
		constexpr IProperty(U&& trans_value)
			noexcept(nothrow_constructible<T, U&&>)
			requires constructible_from<T, U&&>
		: myValue(std::forward<U>(trans_value))
		{}

		constexpr IProperty(const Setter& setter)
			noexcept(nothrow_default_constructibles<T> and nothrow_copy_constructibles<Setter>)
			requires copyable<Setter>
		: myValue(), mySetter(setter)
		{}

		constexpr IProperty(Setter&& setter)
			noexcept(nothrow_default_constructibles<T> and nothrow_move_constructibles<Setter>)
			requires movable<Setter>
		: myValue(), mySetter(std::move(setter))
		{}

		template<convertible_to<T> U>
		constexpr IProperty(U&& trans_value, const Setter& setter)
			noexcept(nothrow_constructible<T, U&&> and nothrow_default_constructibles<T> and nothrow_copy_constructibles<Setter>)
			requires constructible_from<T, U&&> and copyable<Setter>
		: myValue(std::forward<U>(trans_value)), mySetter(setter)
		{}

		template<convertible_to<T> U>
		constexpr IProperty(U&& trans_value, Setter&& setter)
			noexcept(nothrow_constructible<T, U&&> and nothrow_default_constructibles<T> and nothrow_move_constructibles<Setter>)
			requires constructible_from<T, U&&> and movable<Setter>
		: myValue(std::forward<U>(trans_value)), mySetter(std::move(setter))
		{}

		template<convertible_to<T> U, typename X2, typename S2, bool C2, bool R2>
		constexpr IProperty(const IProperty<U, X2, S2, C2, R2>& other)
			noexcept(nothrow_constructible<T, const U&>)
			requires Copyable and C2 and constructible_from<T, const U&>
		: myValue(other.myValue)
		{}

		template<convertible_to<T> U, typename X2, typename S2, bool C2, bool R2>
		constexpr IProperty(IProperty<U, X2, S2, C2, R2>&& other)
			noexcept(nothrow_constructible<T, U&&>)
			requires constructible_from<T, U&&>
		: myValue(std::move(other.myValue))
		{}

		[[nodiscard]]
		constexpr T& value() & noexcept
		{
			return myValue;
		}

		[[nodiscard]]
		constexpr const T& value() const& noexcept
		{
			return myValue;
		}

		[[nodiscard]]
		constexpr T&& value() && noexcept
		{
			return std::move(myValue);
		}

		[[nodiscard]]
		constexpr const T&& value() const&& noexcept
		{
			return std::move(myValue);
		}

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

		template<convertible_to<T> U>
		constexpr IProperty& operator=(U&& value)
			noexcept(nothrow_assignable<U&&, T>)
			requires !Readonly and assignable_from<T, U&&>
		{
			myValue = std::forward<U>(value);
			mySetter(myValue);
			return *this;
		}

		template<convertible_to<T> U, typename X2, typename S2, bool C2, bool R2>
		constexpr IProperty& operator=(const IProperty<U, X2, S2, C2, R2>& other)
			noexcept(nothrow_assignable<const U&, T>)
			requires !Readonly and C2 and assignable_from<T, const U&>
		{
			myValue = other.myValue;
			mySetter(myValue);
			return *this;
		}

		template<convertible_to<T> U, typename X2, typename S2, bool C2, bool R2>
		constexpr IProperty& operator=(IProperty<U, X2, S2, C2, R2>&& other)
			noexcept(nothrow_assignable<U&&, T>)
			requires !Readonly and assignable_from<T, U&&>
		{
			myValue = std::move(other.myValue);
			mySetter(myValue);
			return *this;
		}

	protected:
		T myValue;
		Setter mySetter;
	};

	template<typename T, typename Context, typename Setter>
	IProperty(T, Context, Setter) -> IProperty<T, Context, Setter, copyable<T>, false>;

	template<typename T, typename Context = void>
	IProperty(T, Context) -> IProperty<T, Context, void, copyable<T>, false>;
}
