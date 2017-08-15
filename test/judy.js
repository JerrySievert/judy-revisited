const test = require('tape');

const judy = require('../index');

test('set and get', (t) => {
  t.plan(1);

  let array = new judy();

  array.set('test123', 'hello world');

  t.equal(array.get('test123'), 'hello world', 'the correct value is found');
});

test('delete', (t) => {
  t.plan(2);

  let array = new judy();

  array.set('test456', 'more tests');

  t.equal(array.get('test456'), 'more tests', 'the correct value is found');

  array.delete('test456');

  t.equal(array.get('test456'), undefined, 'undefined is returned');
});

test('size', (t) => {
  t.plan(3);

  let array = new judy();

  array.set('test123', 'hello world');
  array.set('test456', 'goodbye world');

  let size = array.size();

  t.equal(typeof size, 'object', 'and object is returned');
  t.equal(size.memoryAllocated, 42, 'the correct amount of memory is returned');
  t.equal(size.keyCount, 2, 'the correct number of keys is returned');
});
