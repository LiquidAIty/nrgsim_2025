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
    mongoose = require("mongoose"),
    i18n = require("i18next"),
    middleware = require("i18next-http-middleware");

module.exports = function (app, config, i18n, passport) {
    if (!passport || typeof passport.initialize !== "function") {
        throw new Error("❌ Passport is not properly initialized in express.js!");
    }

    app.use(passport.initialize());  // ✅ This should now work
    app.use(passport.session());

    // ✅ Initialize i18n before using it
    i18n.use(middleware.LanguageDetector).init({
        fallbackLng: "en",
        preload: ["en", "es"], // Load more languages if needed
    });

    app.use(middleware.handle(i18n)); // ✅ i18n middleware is now correctly initialized

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
            mongoUrl: process.env.MONGO_URI || "mongodb://localhost/nrgsim", // ✅ Use correct MongoDB URI
            collectionName: "sessions",
        }),
        cookie: { maxAge: 1000 * 60 * 60 * 24 }
    }));

    // ✅ Flash messages
    app.use(flash());

    console.log("✅ Express & Middleware Loaded Successfully!");
};
