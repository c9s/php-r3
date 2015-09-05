

[![Build Status](https://travis-ci.org/c9s/php-r3.svg?branch=master)](https://travis-ci.org/c9s/php-r3)

```sh
phpize
./configure --enable-r3 --with-pcre-dir=/opt/local
make clean
make
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

