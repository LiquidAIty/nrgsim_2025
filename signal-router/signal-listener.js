// signal-router/signal-listener.js
const fs = require('fs');
const path = require('path');
const { SIGNAL_PATH } = require('./signal-writer');

function listenToSignals(onSignal) {
  console.log('[Signal Listener] Watching for signal changes...');
  fs.watchFile(SIGNAL_PATH, { interval: 1000 }, (curr, prev) => {
    if (curr.mtime !== prev.mtime) {
      try {
        const signal = JSON.parse(fs.readFileSync(SIGNAL_PATH));
        console.log('[Signal Listener] New signal detected:', signal);
        onSignal(signal); // Trigger your action handler here
      } catch (err) {
        console.error('[Signal Listener] Failed to parse signal:', err);
      }
    }
  });
}

module.exports = { listenToSignals };
