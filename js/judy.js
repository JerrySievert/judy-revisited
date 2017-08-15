var Module = require('./a.out');

var jg_init = Module.cwrap('jg_init', 'number', ['number', 'number']);
var jg_set = Module.cwrap('jg_set', 'number', ['number', 'string', 'string', 'number']);
var jg_get = Module.cwrap('jg_get', 'string', ['number', 'string']);
var jg_delete = Module.cwrap('jg_delete', null, ['number', 'string']);
var jg_close = Module.cwrap('jg_close', null, ['number']);

function JudyArray (size, depth) {
  this.array = jg_init(size, depth);
}

JudyArray.prototype.get = function get (key) {
  return jg_get(this.array, key);
};

JudyArray.prototype.set = function set (key, value) {
  jg_set(this.array, key, value, value.length);
};

JudyArray.prototype.delete = function del (key) {
  jg_delete(this.array, key);
};

JudyArray.prototype.close = function (close) {
  jg_close(this.array);
};

module.exports = exports = JudyArray;
