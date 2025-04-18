// signal-router/signal-writer.js
const fs = require('fs');
const path = require('path');

const SIGNAL_PATH = path.join(__dirname, '..', 'data', 'signal.json');

function writeSignal(signalObj) {
  const enriched = {
    timestamp: new Date().toISOString(),
    ...signalObj,
  };
  fs.writeFileSync(SIGNAL_PATH, JSON.stringify(enriched, null, 2));
  console.log('[Signal Writer] New signal written:', enriched);
}

module.exports = { writeSignal, SIGNAL_PATH };
