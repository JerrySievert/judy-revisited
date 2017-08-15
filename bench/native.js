var Benchmark = require('benchmark');
var suite = new Benchmark.Suite();
var count = 1000;
var multiplier = 1;

function Exception (message) {
  this.message = message;
}

// add tests
suite.add('Native#Inserts#' + count, function () {
  var arr = {};
  for (var i = 0; i < count; i += 1) {
    arr['key: ' + i] = randomString(i * multiplier);
  }
});
suite.add('Native#Retrieves#' + count, function () {
  var arr = {};
  for (var i = 0; i < count; i += 1) {
    var value = randomString(i * multiplier);
    arr['key: ' + i] = value;
    var key = arr['key: ' + i];
    if (key !== value) {
      throw new Exception('Incorrect');
    }
  }
});
suite.add('Native#RandomRetrieves#' + count, function () {
  var arr = {};
  for (var i = 0; i < count; i += 1) {
    var value = randomString(i * multiplier);
    arr['key: ' + i] = value;
  }
  for (i = 0; i < count; i += 1) {
    var key = 'key: ' + Math.floor(Math.random() * count);
    var val = arr[key];
  }
});
suite.add('Native#Misses#' + count, function () {
  var arr = {};
  for (var i = 0; i < count; i += 1) {
    var value = randomString(i * multiplier);
    arr['key: ' + i] = value;
    var key = arr[value];
    if (key !== undefined) {
      throw new Exception('Incorrect');
    }
  }
});
suite.add('Native#Deletes#' + count, function () {
  var arr = {};
  for (var i = 0; i < count; i += 1) {
    var value = randomString(i * multiplier);
    arr['key: ' + i] = value;
    delete arr['key: ' + i];
  }
});
suite.add('Native#News#' + count, function () {
  var arr = {};
})
  .on('cycle', function (bench) { // add listeners
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
