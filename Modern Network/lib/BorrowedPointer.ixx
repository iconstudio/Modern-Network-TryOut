module;
#include <utility>
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
			requires (not std::is_array_v<U>)
		explicit constexpr BorrowedPointer(U* const& ptr) noexcept
			: myData(ptr)
		{}

		template <convertible_to<T> U = T>
			requires (not std::is_array_v<U>)
		[[nodiscard]]
		constexpr U& operator*() const& noexcept
		{
			return *myData;
		}

		template <convertible_to<T> U = T>
			requires (not std::is_array_v<U>)
		[[nodiscard]]
		constexpr U& operator*() && noexcept
		{
			return *std::exchange(myData, nullptr);
		}

		template <convertible_to<T> U = T>
		[[nodiscard]]
		constexpr volatile U& operator*() const volatile& noexcept
		{
			return *myData;
		}

		template <convertible_to<T> U = T>
		[[nodiscard]]
		constexpr volatile U& operator*() volatile&& noexcept
		{
			return std::exchange(myData, nullptr);
		}

		template <convertible_to<T> U = T>
			requires (not std::is_array_v<U>)
		[[nodiscard]]
		constexpr U* const operator->() const noexcept
		{
			return myData;
		}

		template <convertible_to<T> U = T>
			requires (not std::is_array_v<U>)
		[[nodiscard]]
		constexpr volatile U* const operator->() const volatile noexcept
		{
			return myData;
		}

		constexpr operator T* const& () const& noexcept
		{
			return myData;
		}

		constexpr operator T* () && noexcept
		{
			return std::exchange(myData, nullptr);
		}

		BorrowedPointer(const BorrowedPointer&) noexcept = default;
		BorrowedPointer& operator=(const BorrowedPointer&) noexcept = default;
		BorrowedPointer(BorrowedPointer&&) noexcept = default;
		BorrowedPointer& operator=(BorrowedPointer&&) noexcept = default;

	private:
		T* myData;
	};

	template<notvoids T>
	class [[nodiscard]] BorrowedPointer<std::shared_ptr<T>>
	{
	public:
		constexpr BorrowedPointer() noexcept = default;
		constexpr BorrowedPointer(nullptr_t) noexcept {}

		BorrowedPointer(const std::shared_ptr<T>& ptr)
			: myData(ptr)
		{}

		BorrowedPointer(std::shared_ptr<T>&& ptr) noexcept
			: myData(std::move(ptr))
		{}

		template <convertible_to<T> U = T>
			requires (not std::is_array_v<U>)
		[[nodiscard]]
		U& operator*() const& noexcept
		{
			std::shared_ptr<T> ptr = myData.lock();
			return *ptr;
		}

		template <convertible_to<T> U = T>
			requires (not std::is_array_v<U>)
		[[nodiscard]]
		U& operator*() && noexcept
		{
			std::shared_ptr<T> ptr = std::exchange(myData, nullptr).lock();
			return *ptr;
		}

		[[nodiscard]]
		volatile T& operator*() const volatile& noexcept
		{
			return *static_cast<std::weak_ptr<T>>(myData);
		}

		[[nodiscard]]
		volatile T& operator*() volatile&& noexcept
		{
			return std::exchange(myData, nullptr);
		}

		[[nodiscard]]
		T* const operator->() const noexcept
		{
			std::shared_ptr<T> ptr = myData.lock();
			return ptr.get();
		}

		[[nodiscard]]
		volatile T* const operator->() const volatile noexcept
		{
			return static_cast<std::weak_ptr<T>>(myData).get();
		}

		operator T* const () const noexcept
		{
			return myData.get();
		}

		BorrowedPointer(const BorrowedPointer&) noexcept = default;
		BorrowedPointer& operator=(const BorrowedPointer&) noexcept = default;
		BorrowedPointer(BorrowedPointer&&) noexcept = default;
		BorrowedPointer& operator=(BorrowedPointer&&) noexcept = default;

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
