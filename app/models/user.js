/*jslint node: true */
"use strict";

const mongoose = require('mongoose'); // ✅ Ensure Mongoose is always loaded
const Schema = mongoose.Schema;
const crypto = require('crypto');

const UserSchema = new Schema({
  name: String,
  email: String,
  provider: String,
  hashed_password: String,
  salt: String,
  roles: Array
});

// Make password virtual and generate an encrypted password and salt
UserSchema.virtual('password')
  .set(function(password) {
    this.lcl_password = password;
    this.salt = this.makeSalt();
    this.hashed_password = this.encryptPassword(password);
  })
  .get(function() { return this.lcl_password; });

const fieldIsPresent = function (value) {
  return value && value.length;
};

// the below 4 validations only apply if you are signing up traditionally
UserSchema.path('name').validate(function (name) {
  return name && name.length;
}, 'Name cannot be blank');

UserSchema.path('email').validate(function (email) {
  return email && email.length;
}, 'Email cannot be blank');

UserSchema.path('hashed_password').validate(function (hashed_password) {
  return hashed_password && hashed_password.length;
}, 'Password cannot be blank');

// Pre-save hooks
UserSchema.pre('save', function(next) {
  if (!this.isNew) return next();

  if (!fieldIsPresent(this.password)) {
    next(new Error('Invalid password'));
  } else if (!fieldIsPresent(this.email) && this.provider === 'local') {
    next(new Error('Invalid email'));
  } else {
    next();
  }
});

// Methods
UserSchema.method('authenticate', function(plainText) {
  return this.encryptPassword(plainText) === this.hashed_password;
});

UserSchema.method('makeSalt', function() {
  return Math.round((new Date().valueOf() * Math.random())).toString();
});

UserSchema.method('encryptPassword', function(password) {
  if (!password) return '';
  return crypto.createHmac('sha1', this.salt).update(password).digest('hex');
});

UserSchema.statics.create = function(data) {
  console.log('UserSchema.statics.create: ' + JSON.stringify(data));
  const User = this;
  return new User(data);
};

// ✅ Export the model to ensure it's registered properly
module.exports = mongoose.model('User', UserSchema);
