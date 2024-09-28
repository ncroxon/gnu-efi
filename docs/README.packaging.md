# Packaging gnu-efi

## Set variables per distro configuration

make should be called with the following variables:

- PREFIX (defaults to `/usr/local`)(`%{_prefix}` on RPM distros)
- EXEC_PREFIX (*optional*) (defaults to `$PREFIX`) (`%{_exec_prefix}` on RPM distros)
- LIBDIR (defaults to `$EXEC_PREFIX/lib`) (`%{_libdir}` on RPM distros)
- INCLUDEDIR (*optional*) (defaults to `$PREFIX/include`) (`%{_includedir}` on RPM distros)
- LINUX_HEADERS (*optional*) (**build only**) - location of Linux kernel header files (defaults to `/usr/src/sys/build`)
- DESTDIR (**install only**) - installation output directory (`%{buildroot}` on RPM distros - included in `%make_install`)

Currently, these variables unless stated will need to be set for both build and installation stages.

Those without optional markers I heavily suggest you to set them otherwise files may be located in the wrong directory
for the architecture which you are building for

*The related RPM variables are included here for cross referencing purposes*