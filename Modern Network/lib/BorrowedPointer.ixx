module;
#include <memory>
export module Net.BorrowedPointer;
import Net.Constraints;

export namespace net
{
	template<typename T>
	class [[nodiscard]] BorrowedPointer;

	template<notvoids T>
	class [[nodiscard]] BorrowedPointer<T>
	{
	public:
		constexpr BorrowedPointer() noexcept = default;
		constexpr BorrowedPointer(nullptr_t) noexcept {}

		constexpr BorrowedPointer(T* const& ptr) noexcept
			: myData(ptr)
		{}

		template <convertible_to<T> U>
			requires not std::is_array_v<U>
		constexpr BorrowedPointer(U* const& ptr) noexcept
			: myData(ptr)
		{}

		template <convertible_to<T> U = T>
			requires not std::is_array_v<U>
		[[nodiscard]]
		constexpr U& operator*() const noexcept
		{
			return *myData;
		}

		template <convertible_to<T> U = T>
		[[nodiscard]]
		constexpr volatile U& operator*() const volatile noexcept
		{
			return *myData;
		}

		template <convertible_to<T> U = T>
			requires not std::is_array_v<U>
		[[nodiscard]]
		constexpr U* const operator->() const noexcept
		{
			return myData;
		}
		template <convertible_to<T> U = T>
			requires not std::is_array_v<U>
		[[nodiscard]]
		constexpr volatile U* const operator->() const volatile noexcept
		{
			return myData;
		}


	private:
		T* const myData;
	};

	template<notvoids T>
	class [[nodiscard]] BorrowedPointer<std::shared_ptr<T>>
	{
	public:
		constexpr BorrowedPointer() noexcept = default;
		constexpr BorrowedPointer(nullptr_t) noexcept {}

	private:
		std::weak_ptr<T> myData;
	};

	template<>
	class [[nodiscard]] BorrowedPointer<void>
	{
	public:
		constexpr BorrowedPointer() noexcept = default;
		constexpr BorrowedPointer(nullptr_t) noexcept {}

	private:
	};
}

export
{
	template<typename T>
	void operator delete(net::BorrowedPointer<T>) = delete;
}
