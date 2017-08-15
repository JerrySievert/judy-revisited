# Judy Revisited

Revisiting an old friend, Judy Arrays in node.js.

## Building

```
$ npm install
```

## Using

```js
const JudyArray = require('judy-revisited');

let array = new JudyArray(size, depth);

array.set('some key', 'some value');

let value = array.get('some key');

array.delete('some key');
```

### `set(key, value)`

Sets a `value` for a specified `key`.

_Returns:_ Nothing

### `get(key)`

Retrieves a `value`.

_Returns:_ `value`

### `delete(key)`

Removes a value from the store.

_Returns:_ Nothing

### `keys()`

Gets all `keys` in the store.

_Returns:_ `array` of `keys`

### `size()`

Current size of the store:

```json
{
  "memoryAllocated": 1024,
  "keyCount": 64
}
```

Note that this returns memory allocated for both the `keys` and `values`.

_Returns:_ `object` with a `memoryAllocated` and `keyCount`


## Benchmarks

Benchmarks show the difference between Judy Arrays and native objects.

```
$ node bench/judy.js
Judy#Inserts#1000 x 51.19 ops/sec ±1.61% (64 runs sampled)
Judy#Retrieves#1000 x 50.57 ops/sec ±0.90% (64 runs sampled)
Judy#RandomRetrieves#1000 x 51.83 ops/sec ±1.43% (66 runs sampled)
Judy#Misses#1000 x 50.66 ops/sec ±1.32% (65 runs sampled)
Judy#Deletes#1000 x 52.90 ops/sec ±1.76% (67 runs sampled)
Judy#News#1000 x 2,588,293 ops/sec ±9.72% (63 runs sampled)

$ node bench/native.js
Native#Inserts#1000 x 13.76 ops/sec ±9.51% (35 runs sampled)
Native#Retrieves#1000 x 13.67 ops/sec ±8.51% (29 runs sampled)
Native#RandomRetrieves#1000 x 14.99 ops/sec ±7.35% (28 runs sampled)
Native#Misses#1000 x 67.94 ops/sec ±1.30% (68 runs sampled)
Native#Deletes#1000 x 82.57 ops/sec ±12.12% (53 runs sampled)
Native#News#1000 x 46,864,091 ops/sec ±11.38% (62 runs sampled)
```
