/*jslint node: true */
"use strict";

var mongoose = mongoose || require('mongoose'),
  Schema = mongoose.Schema;

var BuildingParametersSchema = new Schema({
  dimensions: {
    length: Number,
    width: Number,
    height: Number
  },
  insulation: {
    walls: Number,
    roof: Number,
    floor: Number
  },
  phaseChangeMaterial: {
    type: String,
    properties: Object
  },
  zones: [{ type: Object }]
});

var TokenizationSchema = new Schema({
  tokenSymbol: String,
  totalSupply: Number,
  currentPrice: Number,
  tradingEnabled: Boolean,
  smartContractAddress: String
});

var SimulationSchema = new Schema({
  buildingParameters: BuildingParametersSchema,
  tokenization: TokenizationSchema,
  energyResults: {
    annualConsumption: Number,
    peakDemand: Number,
    savings: Number,
    retrofitROI: Number
  },
  tradingStrategy: {
    model: String,
    parameters: Object,
    performance: Object
  },
  userInput: Object,
  finished: { type: Boolean, default: false },
  resultsDirectory: { type: String, default: null },
  resultsFile: { type: String, default: null },
  createdAt: { type: Date, default: Date.now },
  updatedAt: { type: Date, default: Date.now }
});

SimulationSchema.pre('save', function(next) {
  this.updatedAt = Date.now();
  next();
});

SimulationSchema.statics.create = function(data) {
  var Simulation = this;
  return new Simulation(data);
};

mongoose.model('Simulation', SimulationSchema);
