

[![Build Status](https://travis-ci.org/c9s/php-r3.svg?branch=master)](https://travis-ci.org/c9s/php-r3)

## Build

```sh
cd thirdparty/r3 && ./autogen.sh && ./configure && make && cd ../..
phpize
./configure --enable-r3 --with-pcre-dir=/opt/local
make clean
make
```

## Example

```php
$tree = r3_tree_create_persist("app", 10);
if (!r3_tree_is_compiled($tree)) {
    r3_tree_insert($tree, "/foo/bar", ["simplecallback"]);
    r3_tree_insert($tree, "/foo/zoo", ["simplecallback"]);
    r3_tree_insert($tree, "/hack/zzz", ["simplecallback"]);
    r3_tree_compile($tree);
}
$data = r3_tree_match($tree, "/hack/zzz");
print_r($data);
```


```php
$tree = r3_tree_create_persist("app", 10);

$ret = r3_tree_insert($tree, "/foo/bar", "simplecallback");
$ret = r3_tree_insert($tree, "/foo/zoo", ["controller","simplecallback"]);
$ret = r3_tree_insert($tree, "/hack/zzz", ["callable","simplecallback"]);
$ret = r3_tree_compile($tree);
$data = r3_tree_match($tree, "/foo/bar");
$ret = r3_tree_delete_persist("app");
```

