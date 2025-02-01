const express = require('express');
const path = require('path');
const bodyParser = require('body-parser');
const { Sequelize } = require('sequelize');
const app = express();

// Basic middleware
app.use(express.static('public'));
app.use(bodyParser.json());

// Database setup (combining elements from original)
const sequelize = new Sequelize(process.env.DATABASE_URL || 'postgres://postgres:postgres@localhost:5432/nrgsim', {
  dialect: 'postgres',
  ssl: process.env.DATABASE_URL ? { rejectUnauthorized: false } : false
});

// Test database connection (modified from original)
async function initDB() {
  try {
    await sequelize.authenticate();
    console.log('Database connected');
    await sequelize.sync();
    console.log('Database synced');
  } catch (err) {
    console.error('Database connection error:', err);
  }
}

// Basic routes
app.get('/api/simulation/:id', (req, res) => {
  res.json({ id: req.params.id, status: 'success' });
});

app.post('/api/simulation/run', (req, res) => {
  res.json({ status: 'running', data: req.body });
});

// Serve index.html for all other routes
app.get('*', (req, res) => {
  res.sendFile(path.join(__dirname, 'public/index.html'));
});

// Port assignment (from original)
const port = process.env.PORT || 3000;
initDB().then(() => {
  app.listen(port, '0.0.0.0', () => {
    console.log(`Server running on port ${port}`);
  });
});