const express = require('express');
const path = require('path');
const db = require('./db/queries.js')
const app = express(),
      bodyParser = require("body-parser");
      port = 8080;

const fullPath = path.join(__dirname, '../vueapplikation/dist')
app.use(bodyParser.json());
app.use(express.static(fullPath));

db.insertDummys();
app.listen(port, () => {
  console.log(`Server listening on the port::${port}`);
});

app.get('/', (req, res) => {
  res.sendFile(fullPath + 'index.html');
});

app.put('/addbev', async (req, res) => {
  var stringifyJSON = JSON.stringify(req.body);
  const obj = JSON.parse(stringifyJSON);
  await db.insertBeverage(obj);
  res.status(200).send();
});

app.get('/getmf', async (req, res) => {

  const allMf = await db.getAllManufacturers();
  res.send(allMf);
});

app.get('/getcategories', async (req, res) => {

  const allCategories = await db.getAllCategories();
  res.send(allCategories);
});

app.get('/getlogger', async (req, res) => {

  const logger = await db.getLogger();
  res.send(logger);
})

app.get('/getproducts', async (req, res) => {

  const allProducts = await db.getAllProducts();
  res.send(allProducts);
})

app.put('/updatebev', async (req, res) => {
  var stringifyJSON = JSON.stringify(req.body);
  const obj = JSON.parse(stringifyJSON);
  await db.updateBeverage(obj);
  res.status(200).send();
})
app.put('/updatemf', async (req, res) => {
  var stringifyJSON = JSON.stringify(req.body);
  const obj = JSON.parse(stringifyJSON);
  await db.updateMf(obj);
  res.status(200).send();
})

app.delete('/deletebev', async (req, res) => {
  var stringifyJSON = JSON.stringify(req.body);
  const obj = JSON.parse(stringifyJSON);
  await db.deleteBev(obj);
  res.status(200).send();
})

app.put('/addmf', async (req,res) => {
  var stringifyJSON = JSON.stringify(req.body);
  const obj = JSON.parse(stringifyJSON);
  await db.insertManufacturer(obj);
  res.status(200).send();
})

app.put('/addcat', async (req,res) => {
  var stringifyJSON = JSON.stringify(req.body);
  const obj = JSON.parse(stringifyJSON);
  await db.insertCategory(obj);
  res.status(200).send();
})

app.delete('/deletemf', async (req, res) => {
  var stringifyJSON = JSON.stringify(req.body);
  const obj = JSON.parse(stringifyJSON);
  await db.deleteMF(obj);
  res.status(200).send();
})

app.delete('/deletecat', async (req, res) => {
  var stringifyJSON = JSON.stringify(req.body);
  const obj = JSON.parse(stringifyJSON);
  await db.deleteCat(obj);
  res.status(200).send();
})

app.put('/updatecat', async (req, res) => {
  var stringifyJSON = JSON.stringify(req.body);
  const obj = JSON.parse(stringifyJSON);
  await db.updateCat(obj);
  res.status(200).send();
})