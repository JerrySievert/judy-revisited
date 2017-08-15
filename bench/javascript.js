var Benchmark = require('benchmark');
var suite = new Benchmark.Suite();
var count = 1000;
var multiplier = 1;

var judy = require('../js/judy');

function Exception (message) {
  this.message = message;
}

// add tests
suite.add('Judy#Inserts#' + count, function () {
  var arr = new judy(1024, 0);

  for (var i = 0; i < count; i += 1) {
    var value = randomString(i * multiplier);
    arr.set('key: ' + i, value);
  }
  arr.close();
});

suite.add('Judy#Retrieves#' + count, function () {
  var arr = new judy(1024, 0);

  for (var i = 0; i < count; i += 1) {
    var value = randomString(i * multiplier);
    arr.set('key: ' + i, value);
    var key = arr.get('key: ' + i);
    if (key !== value) {
      throw new Exception('Incorrect, expected \"' + value + '\", got \"' + key + '\" for key \"key: ' + i + '\"');
    }
  }
  arr.close();
});

suite.add('Judy#RandomRetrieves#' + count, function () {
  var arr = new judy(1024, 0);

  for (var i = 0; i < count; i += 1) {
    var value = randomString(i * multiplier);
    arr.set('key: ' + i, value);
  }

  for (i = 0; i < count; i += 1) {
    var key = 'key: ' + Math.floor(Math.random() * count);
    var val = arr.get(key);
  }
  arr.close();
});

suite.add('Judy#Misses#' + count, function () {
  var arr = new judy(1024, 0);
  for (var i = 0; i < count; i += 1) {
    var value = randomString(i * multiplier);
    arr.set('key: ' + i, value);
    var key = arr.get(value);
    if (key !== '') {
      throw new Exception('Incorrect');
    }
  }
  arr.close();
});

suite.add('Judy#Deletes#' + count, function () {
  var arr = new judy(1024, 0);

  for (var i = 0; i < count; i += 1) {
    var value = randomString(i * multiplier);
    arr.set('key: ' + i, value);
    arr.delete('key: ' + i);
  }
  arr.close();
});

suite.add('Judy#News#' + count, function () {
  var arr = new judy(1024, 0);
  arr.close();
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
