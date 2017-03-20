# fswrapper
utility function of filesystem for cpp, based on boost.

## build

It assumes the environment where docker is installed.  
Executing the following command will build a container with boost installed.

```
make up
make shell
make build
make test
```

## todo

- [x] create CMakeList.txt
- [x] Scan the directory with **range-based for**.
- [x] Filter files with regex.
- [] Reading text file with Iterator.
- [] ctor by read csv,tsv,and more..., with template.
