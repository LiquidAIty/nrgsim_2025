/*jslint node: true */
"use strict";

require("dotenv").config(); // ✅ Load environment variables FIRST

const express = require("express"),
    session = require("express-session"),
    MongoStore = require("connect-mongo"),
    flash = require("connect-flash"),
    cookieParser = require("cookie-parser"),
    bodyParser = require("body-parser"),
    compression = require("compression"),
    morgan = require("morgan"),
    favicon = require("serve-favicon"),
    path = require("path"),
    mongoose = require("mongoose");

// ✅ Connect to MongoDB using .env variables
mongoose.connect(process.env.MONGO_URI, {
    useNewUrlParser: true,
    useUnifiedTopology: true
}).catch(err => {
    console.error("MongoDB Connection Error:", err);
});

module.exports = function (app, config, passport) { // ✅ Removed `i18n`, since it's unused

  // ✅ Enable Gzip compression
  app.use(compression());

  // ✅ Serve static files
  app.use(express.static(path.join(config.root, "public", "dist")));

  // ✅ Logging middleware
  app.use(morgan("dev"));

  // ✅ Serve favicon (Ensure you have a favicon in `public` folder)
  app.use(favicon(path.join(config.root, "public", "favicon.ico")));

  // ✅ Parse cookies
  app.use(cookieParser());

  // ✅ Parse request bodies
  app.use(bodyParser.urlencoded({ extended: true }));
  app.use(bodyParser.json());

  // ✅ Express-session with MongoDB storage
  app.use(session({
    secret: process.env.SESSION_SECRET || "fallback_secret", // ✅ Use env variable
    resave: false,
    saveUninitialized: true,
    store: MongoStore.create({
      mongoUrl: process.env.MONGO_URI,
      collectionName: "sessions",
    }),
    cookie: { maxAge: 1000 * 60 * 60 * 24 }
  }));

  // ✅ Flash messages
  app.use(flash());

  // ✅ Passport.js Authentication
  app.use(passport.initialize());
  app.use(passport.session());

  // ✅ Improved error handling middleware
  app.use((err, req, res, next) => {
    console.error(`[ERROR] ${err.message}`);
    res.status(err.status || 500).json({ error: err.message || "Internal Server Error" });
  });
};
