# LibUJO Data Object Notation #

**UJO Data Object Notation** is a binary data serialization format for the *Internet of Things* and *machine to machine (M2M)* applications. The name UJO is Esperanto and means container. It is designed for constrained devices, limited network bandwidth and speed. Binary data like images, videos or other multimedia files can be embedded without encoding. Many data types and different containers allow hierarchical data models and typesafe data transfer between different platforms, programming languages and databases.

## Why UJO ? ##

Efficiency in data serialization formats is clearly an important issue. Binary formats are much simpler and more efficient than any text format like JSON or XML no matter how much effort is spent on optimizing. UJO combines a wide variety of datatypes and a flexible hierarchical structure with a simple and clear interface. 
[**read more ...**][1]

## How does UJO store data ?##

An UJO document uses a top level container as root object. The user can choose between three types of containers. 

- **List**: An ordered list of values
- **Map**: Also called a dictionary or associative array is an unordered list of key/value pairs. A key can be any type except a container.
- **Table**: A set of 0 or more rows of values organized in named columns. Column names can be any type of string.

A value in a container can be a container as well to allow hierarchical data structures like JSON or XML.
For more information about the UJO document structure refer to the [specification][2].

## How to get help ? ##

The primary source of information is the [**reference manual**][3]. It includes the [**installation instruction**][10] for different platforms. 

To discuss about UJO, ask questions, get help or receive news about releases, planned features and development progress, a [libujo mailing list][4] is available. [Subscribe][5] to the mailing list to get involved.

For additional information about getting support for libUJO got to [http://www.libujo.org/support/][6].

## libujo-c project ##

The libujo-c project is a C implementation of the UJO specification. Licensed under [LGPL 2.1][7] it can be used for open source and commercial projects according to the [license terms][8]. 

### Project Status ###

Many features are implemented, but some are missing to complete the first release of libujo-c version 1.1. Refer to the [project roadmap][9] for details.

  [1]: http://www.libujo.org/info/
  [2]: http://www.libujo.org/specification/
  [3]: http://www.libujo.org/docs/libujo-c-html/
  [4]: https://groups.google.com/forum/#!forum/libujo
  [5]: https://groups.google.com/forum/#!forum/libujo/join
  [6]: http://www.libujo.org/support/
  [7]: http://www.gnu.org/licenses/licenses.html#LGPL
  [8]: https://github.com/tml21/libujo-c/blob/master/LICENSE
  [9]: http://www.libujo.org/roadmap/
  [10]: http://www.libujo.org/docs/libujo-c-html/md_ujo_install.html