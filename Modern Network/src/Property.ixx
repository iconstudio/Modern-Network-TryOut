export module Net.Property;
import :IProperty;
import Net.Constraints;

export namespace net
{
	template<movable T, typename Context = void, bool Copyable = copyable<T>>
	using Property = IProperty<T, Context, false, Copyable, false>;

	template<movable T, typename Context = void, bool Copyable = copyable<T>>
	using CustomProperty = IProperty<T, Context, true, Copyable, false>;

	template<movable T, typename Context = void, bool Custom = false>
	using ReadonlyProperty = IProperty<T, Context, Custom, false, true>;
}
