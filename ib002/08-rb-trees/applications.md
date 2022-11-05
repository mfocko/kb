---
title: Použití červeno-černých stromů
description: |
  Ukázka použití červeno-černých stromů pro implementaci množiny nebo mapy
  a ukázky jednotlivých implementací ve standardních knižnicích známých jazyků.
---

## Použití

Červeno-černé stromy jsou celkem oblíbené pro implementaci ADT množiny nebo slovníku za předpokladu, že nad vkládanými klíči existuje uspořádání. Jazyky níže implementují dané datové struktury v 2 variantách a to:

- seřazené: používají na pozadí právě červeno-černý strom
- neseřazené: používají na pozadí hašovací tabulku

Pro srovnání, jak jsme si říkali na cvičení, červeno-černý strom má operace hledání, vkládání a mazání v časové složitosti $\mathcal{O}(\log n)$. Na druhou stranu hašovací tabulka má ideálně konstantní časovou složitost, ale v nejhorším případě (detaily na posledním cvičení v semestru) je to bohužel $\mathcal{O}(n)$.

Výše jsme si ukázali nějaké předpoklady nutné pro hašovací tabulku i červeno-černý strom. Co je tedy lepší?

- červeno-černý strom nám poskytuje _stabilní časovou složitost_, ale za cenu požadavku _uspořádání_ nad prvky
- hašovací tabulka nám poskytuje _pomyslnou perfektní časovou složitost_

## Různé implementace

Pro ukázku použití červeno-černých stromů v implementacích standardních knihoven
jsme vybrali několik jazyků.

Pokud Vás zajímají různé implementace, tak bychom doporučili „prohrabávat“ se přes ně v následujícím pořadí: `C# → Java → C++`. Důvod pro zvolené pořadí vychází z toho, že C# implementace je poměrně čitelná a obsahuje množství vysvětlujících komentářů. Implementace v Javě je stejně čitelná, ačkoli již s minimem komentářů, které se maximálně odkazují na CLRS. C++ implementace je „značně poznačená“ podtržítky ;)

### C++

V C++ si můžeme vybrat mezi 2 různými implementacemi (`clang` nebo `gcc`).

#### clang

Hlavičkové soubory, které používáme při práci s množinou nebo slovníkem (zajímavé sekce jsou vytaženy):

- [`map`](https://github.com/llvm/llvm-project/blob/main/libcxx/include/map)

  ```cpp
  template <class _Key, class _Tp, class _Compare = less<_Key>,
            class _Allocator = allocator<pair<const _Key, _Tp> > >
  class _LIBCPP_TEMPLATE_VIS map
  {
  public:
      // types:
      typedef _Key                                     key_type;
      typedef _Tp                                      mapped_type;
      typedef pair<const key_type, mapped_type>        value_type;

  // …

  private:
      typedef __tree<__value_type, __vc, __allocator_type>   __base;
  ```

- [`set`](https://github.com/llvm/llvm-project/blob/main/libcxx/include/set)

  ```cpp
  template <class _Key, class _Compare = less<_Key>,
            class _Allocator = allocator<_Key> >
  class _LIBCPP_TEMPLATE_VIS set
  {
  public:
      // types:
      typedef _Key                                     key_type;
      typedef key_type                                 value_type;

  // …

  private:
      typedef __tree<value_type, value_compare, allocator_type> __base;
  ```

U obou hlaviček si můžeme všimnout, že deklarují nějaký soukromý typ `__base`, který je aliasem pro `__tree`. Ten nás pak vede k hlavičce [`__tree`](https://github.com/llvm/llvm-project/blob/main/libcxx/include/__tree).

Výňatek:

```cpp
/*

_NodePtr algorithms

The algorithms taking _NodePtr are red black tree algorithms.  Those
algorithms taking a parameter named __root should assume that __root
points to a proper red black tree (unless otherwise specified).

…

*/
```

#### gcc

Pro `gcc` je postup téměř stejný. Pro změnu v hlavičkách `map` a `set` nenajdeme nic, deklarace jsou až v hlavičkových souborech:

- [`bits/stl_map.h`](https://gcc.gnu.org/git/?p=gcc.git;a=blob;f=libstdc%2B%2B-v3/include/bits/stl_map.h;h=9c2b0745673431b4b396ba27982170478838137e;hb=HEAD)
- [`bits/stl_set.h`](https://gcc.gnu.org/git/?p=gcc.git;a=blob;f=libstdc%2B%2B-v3/include/bits/stl_set.h;h=9c2b0745673431b4b396ba27982170478838137e;hb=HEAD)

V obou se zase odkazuje na nějakou hlavičku [`bits/stl_tree.h`](https://gcc.gnu.org/git/?p=gcc.git;a=blob;f=libstdc%2B%2B-v3/include/bits/stl_tree.h;h=a4de61417652a288e361a55fcc8bb7a9838c58a5;hb=HEAD), zase výňatek:

```cpp
  // Red-black tree class, designed for use in implementing STL
  // associative containers (set, multiset, map, and multimap). The
  // insertion and deletion algorithms are based on those in Cormen,
  // Leiserson, and Rivest, Introduction to Algorithms (MIT Press,
  // 1990), except that
  //
  // (1) the header cell is maintained with links not only to the root
  // but also to the leftmost node of the tree, to enable constant
  // time begin(), and to the rightmost node of the tree, to enable
  // linear time performance when used with the generic set algorithms
  // (set_union, etc.)
  //
  // (2) when a node being deleted has two children its successor node
  // is relinked into its place, rather than copied, so that the only
  // iterators invalidated are those referring to the deleted node.

  enum _Rb_tree_color { _S_red = false, _S_black = true };

  struct _Rb_tree_node_base
  {
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef const _Rb_tree_node_base* _Const_Base_ptr;

    _Rb_tree_color	_M_color;
    _Base_ptr		_M_parent;
    _Base_ptr		_M_left;
    _Base_ptr		_M_right;

    static _Base_ptr
    _S_minimum(_Base_ptr __x) _GLIBCXX_NOEXCEPT
    {
      while (__x->_M_left != 0) __x = __x->_M_left;
      return __x;
    }

    static _Const_Base_ptr
    _S_minimum(_Const_Base_ptr __x) _GLIBCXX_NOEXCEPT
    {
      while (__x->_M_left != 0) __x = __x->_M_left;
      return __x;
    }

    static _Base_ptr
    _S_maximum(_Base_ptr __x) _GLIBCXX_NOEXCEPT
    {
      while (__x->_M_right != 0) __x = __x->_M_right;
      return __x;
    }

    static _Const_Base_ptr
    _S_maximum(_Const_Base_ptr __x) _GLIBCXX_NOEXCEPT
    {
      while (__x->_M_right != 0) __x = __x->_M_right;
      return __x;
    }
```

Tady už taky vidíme nějaký kód pro nalezení minima/maxima ve stromě. Mimo jiné
ještě existuje [`tree.cc`](https://gcc.gnu.org/git/?p=gcc.git;a=blob;f=libstdc%2B%2B-v3/src/c%2B%2B98/tree.cc;h=fd14991589a57c6aa847f57105a938cd2bf4df6f;hb=HEAD), kde je lze nalézt třeba funkci s následující hlavičkou:

```cpp
void
_Rb_tree_insert_and_rebalance(const bool          __insert_left,
                              _Rb_tree_node_base* __x,
                              _Rb_tree_node_base* __p,
                              _Rb_tree_node_base& __header) throw ();
```

### Java

V Javě jsou pro nás klíčové implementace [`TreeSet`](https://github.com/openjdk/jdk/blob/master/src/java.base/share/classes/java/util/TreeSet.java) a [`TreeMap`](https://github.com/openjdk/jdk/blob/master/src/java.base/share/classes/java/util/TreeMap.java).

V implementaci `TreeSet` si můžete povšimnout:

```java
public class TreeSet<E> extends AbstractSet<E>
    implements NavigableSet<E>, Cloneable, java.io.Serializable
{
    /**
     * The backing map.
     */
    private transient NavigableMap<E,Object> m;

    // Dummy value to associate with an Object in the backing Map
    private static final Object PRESENT = new Object();
```

`TreeSet` v Javě tedy používá na pozadí `TreeMap` (což je vidět ve výchozím konstruktoru, kde se volá konstruktor přebírající `NavigableMap<E, Object>`, a je mu předáno `new TreeMap<>()`).

Co se týče `TreeMap`, tak hned ze začátku definice `TreeMap` je vidět:

```java
public class TreeMap<K,V>
    extends AbstractMap<K,V>
    implements NavigableMap<K,V>, Cloneable, java.io.Serializable
{
    /**
     * The comparator used to maintain order in this tree map, or
     * null if it uses the natural ordering of its keys.
     *
     * @serial
     */
    @SuppressWarnings("serial") // Conditionally serializable
    private final Comparator<? super K> comparator;

    private transient Entry<K,V> root;
```

Takže máme „nějaký kořen“ typu `Entry<K,V>`. Zkusíme si najít definici daného typu…

```java
    // Red-black mechanics

    private static final boolean RED   = false;
    private static final boolean BLACK = true;

    /**
     * Node in the Tree.  Doubles as a means to pass key-value pairs back to
     * user (see Map.Entry).
     */

    static final class Entry<K,V> implements Map.Entry<K,V> {
        K key;
        V value;
        Entry<K,V> left;
        Entry<K,V> right;
        Entry<K,V> parent;
        boolean color = BLACK;
```

A máme RB-tree.

(Implementace vychází z projektu OpenJDK.)

### C#

V C# se zaměříme na nejnovější vydání (.NET), které je open-source a podporováno i na operačních systémech založených na Linuxu.

Nejdříve se podíváme na implementaci slovníku ([`SortedDictionary`](https://github.com/dotnet/runtime/blob/main/src/libraries/System.Collections/src/System/Collections/Generic/SortedDictionary.cs)).

```csharp
    public class SortedDictionary<TKey, TValue> : IDictionary<TKey, TValue>, IDictionary, IReadOnlyDictionary<TKey, TValue> where TKey : notnull
    {
        [NonSerialized]
        private KeyCollection? _keys;
        [NonSerialized]
        private ValueCollection? _values;

        private readonly TreeSet<KeyValuePair<TKey, TValue>> _set; // Do not rename (binary serialization)
```

Na první pohled máme problém, protože `TreeSet` není `SortedSet`, který by jsme čekali. Když se přesuneme na konec souboru, tak zjistíme, že `TreeSet` je jenom _backward-compatible wrapper_ pro `SortedSet`.

Přesuneme se k [`SortedSet`](https://github.com/dotnet/runtime/blob/main/src/libraries/System.Collections/src/System/Collections/Generic/SortedSet.cs). A hned ze začátku vidíme:

```cs
    // A binary search tree is a red-black tree if it satisfies the following red-black properties:
    // 1. Every node is either red or black
    // 2. Every leaf (nil node) is black
    // 3. If a node is red, the both its children are black
    // 4. Every simple path from a node to a descendant leaf contains the same number of black nodes
    //
    // The basic idea of a red-black tree is to represent 2-3-4 trees as standard BSTs but to add one extra bit of information
    // per node to encode 3-nodes and 4-nodes.
    // 4-nodes will be represented as:   B
    //                                 R   R
    //
    // 3 -node will be represented as:   B     or     B
    //                                 R   B        B   R
    //
    // For a detailed description of the algorithm, take a look at "Algorithms" by Robert Sedgewick.

    internal enum NodeColor : byte
    {
        Black,
        Red
    }

    internal delegate bool TreeWalkPredicate<T>(SortedSet<T>.Node node);

    internal enum TreeRotation : byte
    {
        Left,
        LeftRight,
        Right,
        RightLeft
    }
```

Vysvětlení v komentáři trochu předbíhá náplň cvičení zaměřeného na B-stromy ;)

## Vztah mezi množinou a mapou

Při každé implementaci ve standardní knihovně jsme si mohli všimnout, že strom implementuje vždy jenom jeden typ:

| Jazyk | Způsob implementace                             |
| ----- | ----------------------------------------------- |
| C++   | mapa ukládá dvojice do množiny                  |
| Java  | množina ukládá prvky s „dummy“ hodnotou do mapy |
| C#    | mapa ukládá dvojice do množiny                  |

Mapa vyžaduje, aby každý klíč měl přiřazenou právě jednu hodnotu, tedy klíče jsou navzájem mezi sebou unikátní. To nám umožňuje organizovat klíče do množiny, zde ale narazíme na nepříjemný problém spočívající v tom, že musíme do množiny vkladat dvojice prvků: `(key, value)`. Tenhle přístup má ale zásadní problém:

```py showLineNumbers
# let's represent dictionary/map as a set
set_of_values = set()

# let's insert few pairs
set_of_values.add((1, 2))
set_of_values.add((0, 42))

# let's set key 1 to value 6
set_of_values.add((1, 6))

set_of_values
```

A dostaneme:

```
{(1, 6), (1, 2), (0, 42)}
```

V jednotlivých implementacích, které jste mohli vidět výše, se využívá nasledující, když:

- _mapa ukládá dvojice do množiny_: Dvojice je obalená v samostatním typu, který porovnává jenom klíče
- _množina ukládá klíče do mapy_: V mapě se ignorují hodnoty přiřazené klíčům
