# Rocket Subsystem Register Manager

A menu-driven C program for managing subsystem records in a fixed-capacity collection. Each record stores a name, a packed status byte, and an unsigned data value.

## Features

- Add, find, display, and remove subsystem records.
- Update power, data, activity, error, performance, and resource fields using bit masks.
- Set and retrieve data values in hexadecimal form.
- Filter records using eight-character patterns containing `0`, `1`, and `*` wildcards.
- Store up to 100 records without dynamically allocating the collection.

## Build and run

A C11 compiler and the POSIX `strcasecmp` function are required.

```sh
make
./main
make clean
```

## Source files

- `main.c`: menu and operation dispatch.
- `subsys.c`: status and data operations, with input helpers.
- `subsys_collection.c`: collection operations and filtering.
- `subsystem.h`: structures, constants, and declarations.

Data uses `unsigned int`; its width depends on the target platform. Retrieving data clears its stored value and data-available status bit.
