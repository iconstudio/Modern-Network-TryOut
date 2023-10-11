export module Net.Property;
import Net.Constraints;
import <type_traits>;
import <functional>;

export namespace net
{
	template<typename T, typename Setter, bool Copyable, bool Readonly>
	class IProperty;

	template<movable T, bool Copyable, bool Readonly>
	class IProperty<T, void, Copyable, Readonly>
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

		template<convertible_to<T> U, typename S2, bool C2, bool R2>
		constexpr IProperty(const IProperty<U, S2, C2, R2>& other)
			noexcept(nothrow_constructible<T, const U&>)
			requires Copyable and C2 and copy_constructibles<T, U> and constructible_from<T, const U&>
		: myValue(other.myValue)
		{}

		template<convertible_to<T> U, typename S2, bool C2, bool R2>
		constexpr IProperty(IProperty<U, S2, C2, R2>&& other)
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
		explicit constexpr operator T& () & noexcept
		{
			return myValue;
		}

		[[nodiscard]]
		explicit constexpr operator const T& () const& noexcept
		{
			return myValue;
		}

		[[nodiscard]]
		explicit constexpr operator T && () && noexcept
		{
			return std::move(myValue);
		}

		[[nodiscard]]
		explicit constexpr operator const T && () const&& noexcept
		{
			return std::move(myValue);
		}

		template<convertible_to<T> U, typename S2, bool C2, bool R2>
		constexpr IProperty& operator=(const IProperty<U, S2, C2, R2>& other)
			noexcept(nothrow_assignable<const U&, T>)
			requires !Readonly and C2 and copy_assignables<T, U> and constructible_from<T, const U&>
		{
			myValue = other.myValue;
			return *this;
		}

		template<convertible_to<T> U, typename S2, bool C2, bool R2>
		constexpr IProperty& operator=(IProperty<U, S2, C2, R2>&& other)
			noexcept(nothrow_assignable<U&&, T>)
			requires !Readonly and move_assignables<T, U> and constructible_from<T, U&&>
		{
			myValue = std::move(other.myValue);
			return *this;
		}

	protected:
		T myValue;
	};

	template<movable T, invocables<T&> Setter, bool Copyable, bool Readonly>
	class IProperty<T, Setter, Copyable, Readonly>
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

		template<convertible_to<T> U, typename S2, bool C2, bool R2>
		constexpr IProperty(const IProperty<U, S2, C2, R2>& other)
			noexcept(nothrow_constructible<T, const U&>)
			requires Copyable and C2 and copy_constructibles<T, U> and constructible_from<T, const U&>
		: myValue(other.myValue)
		{}

		template<convertible_to<T> U, typename S2, bool C2, bool R2>
		constexpr IProperty(IProperty<U, S2, C2, R2>&& other)
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
		explicit constexpr operator T& () & noexcept
		{
			return myValue;
		}

		[[nodiscard]]
		explicit constexpr operator const T& () const& noexcept
		{
			return myValue;
		}

		[[nodiscard]]
		explicit constexpr operator T && () && noexcept
		{
			return std::move(myValue);
		}

		[[nodiscard]]
		explicit constexpr operator const T && () const&& noexcept
		{
			return std::move(myValue);
		}

		template<convertible_to<T> U, typename S2, bool C2, bool R2>
		constexpr IProperty& operator=(const IProperty<U, S2, C2, R2>& other)
			noexcept(nothrow_assignable<const U&, T>)
			requires !Readonly and C2 and copy_assignables<T> and assignable_from<T, const U&>
		{
			myValue = other.myValue;
			return *this;
		}

		template<convertible_to<T> U, typename S2, bool C2, bool R2>
		constexpr IProperty& operator=(IProperty<U, S2, C2, R2>&& other)
			noexcept(nothrow_assignable<U&&, T>)
			requires !Readonly and move_assignables<T> and assignable_from<T, U&&>
		{
			myValue = std::move(other.myValue);
			return *this;
		}

	protected:
		T myValue;
		Setter mySetter;
	};

	template<movable T, typename Setter = void, bool Copyable = copyable<T>>
	using Property = IProperty<T, Setter, Copyable, false>;

	template<typename T, typename Setter>
	IProperty(T, Setter) -> IProperty<T, Setter, copyable<T>, false>;

	template<typename T>
	IProperty(T) -> IProperty<T, void, copyable<T>, false>;

	template<movable T, typename Setter = void>
	using ReadonlyProperty = IProperty<T, Setter, false, true>;
}
