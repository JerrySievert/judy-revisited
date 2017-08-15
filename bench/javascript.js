var Benchmark = require('benchmark');
var suite = new Benchmark.Suite();
var count = 1000;
var multiplier = 1;

var judy = require('../javascript/judy');

// add tests
suite.add('Judy#Inserts#' + count, function () {
  var arr = judy.jg_init(1024, 0);

  for (var i = 0; i < count; i += 1) {
    var value = randomString(i * multiplier);
    judy.jg_set(arr, 'key: ' + i, val, value.length);
  }
  judy.jg_close(arr);
});

suite.add('Judy#Retrieves#' + count, function () {
  var arr = judy.jg_init(1024, 0);

  for (var i = 0; i < count; i += 1) {
    var value = randomString(i * multiplier);
    judy.jg_set(arr, 'key: ' + i, value, value.length);
    var key = judy.jg_get(arr, 'key: ' + i);
    if (key !== value) {
      throw new Exception('Incorrect');
    }
  }
  judy.jg_close(arr);
});

suite.add('Judy#RandomRetrieves#' + count, function () {
  var arr = judy.jg_init(1024, 0);

  for (var i = 0; i < count; i += 1) {
    var value = randomString(i * multiplier);
    judy.jg_set(arr, 'key: ' + i, value, value.length);
  }

  for (i = 0; i < count; i += 1) {
    var key = 'key: ' + Math.floor(Math.random() * count);
    var val = judy.jg_get(arr, key);
  }
  judy.jg_close(arr);
});

suite.add('Judy#Misses#' + count, function () {
  var arr = judy.jg_init(1024, 0);
  for (var i = 0; i < count; i += 1) {
    var value = randomString(i * multiplier);
    judy.set(arr, 'key: ' + i, value, value.length);
    var key = judy.get(arr, value);
    if (key !== undefined) {
      throw new Exception('Incorrect');
    }
  }
  judy.jg_close(arr);
});

suite.add('Judy#Deletes#' + count, function () {
  var arr = judy.jg_init(1024, 0);

  for (var i = 0; i < count; i += 1) {
    var value = randomString(i * multiplier);
    judy.set(arr, 'key: ' + i, value, value.length);
    judy.delete(arr, 'key: ' + i);
  }
  judy.jg_close(arr);
});

suite.add('Judy#News#' + count, function () {
  var arr = judy.jg_init(1024, 0);
  judy.jg_close(arr);
});

suite.on('cycle', function (bench) { // add listeners
    console.log(String(bench.target));
  })
  .run(false); // run async
function randomString (length) {
  var parts = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890';
  var str = '';
  for (var x = 0; x < length; x += 1) {
    var i = Math.floor(Math.random() * 62);
    str += parts.charAt(i);
  }
  return str;
}
