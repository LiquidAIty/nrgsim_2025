/*jslint node: true */
"use strict";

require('dotenv').config(); // ✅ Load environment variables first

const express = require('express');
const passport = require("passport"); 
require("./config/passport")(passport); // ✅ Load Passport configuration

const i18n = require('i18next'); // ✅ Ensure i18n is required before it's used
const Backend = require('i18next-fs-backend'); // ✅ Fix missing backend issue
const env = process.env.NODE_ENV || 'development';
const config = require('./config/config')[env];
const auth = require('./config/middleware/authorization');
const mongoose = require('mongoose');

// ✅ Set up database connection (Remove deprecated options)
const dbURI = process.env.MONGODB_URI || config.db;
mongoose.connect(dbURI, {}) // ✅ Removed useNewUrlParser & useUnifiedTopology
.then(() => console.log("✅ Connected to MongoDB Atlas!"))
.catch(err => {
    console.error("❌ MongoDB connection error:", err);
    process.exit(1); // Exit if DB fails
});

// ✅ Set up models before using them
require('./app/models/user');        
require('./app/models/simulation');

// ✅ Initialize Express after everything else
var app = express();
require('./config/express')(app, config, i18n, passport);

// ✅ Fix i18n "No backend" warning
i18n.use(Backend) // ✅ Load translation files
    .use(require('i18next-http-middleware').LanguageDetector) // ✅ Keep language detection
    .init({
        fallbackLng: 'en',
        preload: ['en', 'es'],
        debug: true,
        backend: {
            loadPath: "./locales/{{lng}}.json" // ✅ Tell i18n where translations are
        }
    });

// ✅ Initialize i18n and set up routes
i18n.init({}, function(t) {
    require('./config/routes')(app, t, passport, auth);
});

// Start the app by listening on <port>
var port = process.env.PORT || 3000;
app.listen(port, () => {
    console.log(`🚀 App listening on port ${port}`);
});
