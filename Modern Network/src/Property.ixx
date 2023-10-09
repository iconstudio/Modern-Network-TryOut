export module Net.Property;
import Net.Constraints;
import <type_traits>;

export namespace net
{
	template<movable T, bool Copyable, bool Readonly>
	class IProperty
	{
	public:
		constexpr IProperty()
			noexcept(nothrow_default_constructibles<T>) = default;
		virtual constexpr ~IProperty()
			noexcept(nothrow_destructibles<T>) = default;

		template<convertible_to<T> U>
		constexpr IProperty(U&& trans_value)
			noexcept(nothrow_constructibles<T, U&&>) requires constructible_from<T, U&&>
			: myValue(std::forward<U>(trans_value))
		{}

		template<convertible_to<T> U, bool C2, bool R2>
		constexpr IProperty(const IProperty<U, C2, R2>& other)
			noexcept(nothrow_constructibles<T, const U&>)
			requires Copyable and C2 and copy_constructibles<T, U> and constructible_from<T, const U&>
		: myValue(other.myValue)
		{}

		template<convertible_to<T> U, bool C2, bool R2>
		constexpr IProperty(IProperty<U, C2, R2>&& other)
			noexcept(nothrow_constructibles<T, U&&>)
			requires move_constructibles<T, U> and constructible_from<T, U&&>
		: myValue(std::move(other.myValue))
		{}

		template<convertible_to<T> U, bool C2, bool R2>
		constexpr IProperty& operator=(const IProperty<U, C2, R2>&other)
			noexcept(nothrow_assignables<const U&, T>)
			requires !Readonly and C2 and copy_assignables<T, U> and constructible_from<T, const U&>
		{
			myValue = other.myValue;
			return *this;
		}

		template<convertible_to<T> U, bool C2, bool R2>
		constexpr IProperty& operator=(IProperty<U, C2, R2>&& other)
			noexcept(nothrow_assignables<U&&, T>)
			requires !Readonly and move_assignables<T, U> and constructible_from<T, U&&>
		{
			myValue = std::move(other.myValue);
			return *this;
		}

	protected:
		T myValue;
	};

	template<movable T, bool Copyable = copyable<T>>
	using Property = IProperty<T, Copyable, false>;

	template<movable T>
	using ReadonlyProperty = IProperty<T, false, true>;
}
