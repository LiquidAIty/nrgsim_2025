const LocalStrategy = require("passport-local").Strategy;
const passport = require("passport");
const User = require("../app/models/user"); // ✅ Explicitly require User model

module.exports = function (passport) {
    passport.serializeUser((user, done) => {
        done(null, user.id);
    });

    passport.deserializeUser((id, done) => {
        User.findById(id, (err, user) => {
            done(err, user);
        });
    });

    passport.use(new LocalStrategy({
        usernameField: "email",
        passwordField: "password"
    }, (email, password, done) => {
        User.findOne({ email: email }, (err, user) => {
            if (err) return done(err);
            if (!user) return done(null, false, { message: "Unknown user" });
            if (!user.authenticate(password)) return done(null, false, { message: "Invalid password" });
            return done(null, user);
        });
    }));
};
