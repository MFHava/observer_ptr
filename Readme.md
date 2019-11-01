# `boost::observer_ptr`

> `observer_ptr<T>` is "... a (not very) smart pointer type that takes no ownership responsibility for its pointees, i.e., for the objects it observes.
> As such, it is intended as a near drop-in replacement for raw pointer types, with the advantage that, as a vocabulary type, it indicates its intended use without need for detailed analysis by code readers."
> -- <cite>Walter E. Brown, N3840</cite>

See documentation [here](doc/observer_ptr.md).

(c) Bernhard Manfred Gruber & Michael Florian Hava

Released under the Boost Software License - Version 1.0, see "LICENSE_1_0.txt" for details.

This library and documentation is largly taken from the Working Draft, C++ Extensions for Library Fundamentals, Version 3 ([N4806](http://wg21.link/N4806).)
The original design of `observer_ptr<T>` was proposed by Walter E. Brown ([N3840](wg21.link/n3840)).
