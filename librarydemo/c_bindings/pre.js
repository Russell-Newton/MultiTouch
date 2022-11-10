var Module = {
  preRun : [],
  postRun : [],
  print : function(text) { console.log('gesturelib stdout: ' + text) },
  printErr : function(text) { alert('gesturelib stderr: ' + text) },
  onRuntimeInitialized : function() {
    let event = new Event("emscriptenready");
    document.dispatchEvent(event);
  }
};
