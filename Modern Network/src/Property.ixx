export module Net.Property;
import :IProperty;
import Net.Constraints;

export namespace net
{
	template<movable T, typename Context = void, typename Setter = void, bool Copyable = copyable<T>>
	using Property = IProperty<T, Context, Setter, Copyable, false>;

	template<movable T, typename Context = void, typename Setter = void>
	using ReadonlyProperty = IProperty<T, Context, Setter, false, true>;
}
