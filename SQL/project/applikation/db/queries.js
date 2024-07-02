const { Pool } = require('pg')
const { release } = require('process')
const { callWithAsyncErrorHandling } = require('vue')
const pool = new Pool({
  user: 'c5dv202_vt22_c19mhg',
  host: 'postgres.cs.umu.se',
  database: 'c5dv202_vt22_c19mhg',
  password: '99pHFJHcxvUw',
  port: 5432,
})

const tables = `

DO $$
  DECLARE
    r RECORD;
BEGIN
  FOR r IN
    (
      SELECT table_name
      FROM information_schema.tables
      WHERE table_schema=current_schema()
    )
  LOOP
     EXECUTE 'DROP TABLE IF EXISTS ' || quote_ident(r.table_name) || ' CASCADE';
  END LOOP;
END $$ ;

CREATE TABLE category (
  category VARCHAR(255) NOT NULL,
  category_description VARCHAR(255),
  PRIMARY KEY(category)
);

INSERT INTO category VALUES ('Lager Beer', 'taste very good, nice');
INSERT INTO category VALUES ('IPA', 'Very good beer yes');
INSERT INTO category VALUES ('Stout', 'very dark beer, taste strange');
INSERT INTO category VALUES ('APA', 'taste very good, nice');
INSERT INTO category VALUES ('Red Wine', 'Wine that is red');
INSERT INTO category VALUES ('White Wine', 'Wine that is white');
INSERT INTO category VALUES ('Sparkling Wine', 'White wine but bubbly');
INSERT INTO category VALUES ('Hard liquor', 'Drink with caution');

CREATE TABLE manufacturer (
  manufacturer VARCHAR(255) NOT NULL,
  country VARCHAR(255),
  adress VARCHAR(255),
  PRIMARY KEY(manufacturer)
);

INSERT INTO manufacturer VALUES ('Carlsberg', 'Danmark', 'damnvarksvägen');
INSERT INTO manufacturer VALUES ('Gustaf hemmbränning AB', 'Sverige', 'Tvistevägen');
INSERT INTO manufacturer VALUES ('Åbro', 'Sverige', 'ölvägen 10');
INSERT INTO manufacturer VALUES ('Pabst Brewing Company', 'USA', 'Milwaukeeroad');

CREATE TABLE product (
  product VARCHAR(255) NOT NULL,
  manufacturer VARCHAR(255) NOT NULL,
  category VARCHAR(255) NOT NULL,
  product_description VARCHAR(255),
  product_price VARCHAR(255),
  product_abv VARCHAR(255),
  product_volume VARCHAR(255),
  rating VARCHAR(255),
  PRIMARY KEY(product),
  FOREIGN KEY(manufacturer) REFERENCES manufacturer(manufacturer) on update cascade, 
  FOREIGN KEY(category) REFERENCES category(category) on update cascade
);

CREATE TABLE logger (
  name VARCHAR(255) NOT NULL,
  action VARCHAR(255) NOT NULL,
  time VARCHAR(255) NOT NULL
);

DROP TRIGGER IF EXISTS insertedProduct ON product;
DROP FUNCTION IF EXISTS addLOGGERinsert();
DROP TRIGGER IF EXISTS updatedProduct ON product;
DROP FUNCTION IF EXISTS addLOGGERupdate();
DROP TRIGGER IF EXISTS deletedProduct ON product;
DROP FUNCTION IF EXISTS addLOGGERdelete();

CREATE FUNCTION addLOGGERinsert()
  RETURNS trigger AS 
  $$
  BEGIN 
      INSERT INTO logger(name, action, time) VALUES(NEW.product, 'Inserted', to_char(NOW(), 'DD-MON-YYYY HH24:MI:SS'));
      RETURN NEW;
  END;

  $$
  LANGUAGE 'plpgsql';

  CREATE FUNCTION addLOGGERupdate()
  RETURNS trigger AS 
  $$
  BEGIN 
  
      INSERT INTO logger(name, action, time) VALUES(OLD.product, 'Updated', to_char(NOW(), 'DD-MON-YYYY HH24:MI:SS'));
      RETURN OLD;
  END;

  $$
  LANGUAGE 'plpgsql';
  CREATE FUNCTION addLOGGERdelete()
  RETURNS trigger AS 
  $$
  BEGIN 
  
      INSERT INTO logger(name, action, time) VALUES(OLD.product, 'Deleted', to_char(NOW(), 'DD-MON-YYYY HH24:MI:SS'));
      RETURN OLD;
  END;
  $$
  LANGUAGE 'plpgsql';

CREATE TRIGGER insertedProduct AFTER INSERT ON product
FOR EACH ROW 
EXECUTE PROCEDURE addLOGGERinsert();
CREATE TRIGGER updatedProduct AFTER UPDATE ON product
FOR EACH ROW 
EXECUTE PROCEDURE addLOGGERupdate();
CREATE TRIGGER deletedProduct AFTER DELETE ON product
FOR EACH ROW 
EXECUTE PROCEDURE addLOGGERdelete();

INSERT INTO product VALUES ('Carslberg beer', 'Carlsberg', 'Lager Beer', 'Danish beer, taste of wienerbröd', '10', '5.2', '33 cl', 5);
INSERT INTO product VALUES ('Bäverhojt', 'Gustaf hemmbränning AB', 'Hard liquor', 'No description needed', '299', '50', '70 cl', 9);
INSERT INTO product VALUES ('Blue Ribbon', 'Pabst Brewing Company', 'Lager Beer', 'American beer, very tasy', '15', '5.0', '50 cl', 3);
INSERT INTO product VALUES ('Nollevin', 'Gustaf hemmbränning AB', 'White Wine', 'Wine made in gustaf bathtub, very good', '100', '15.0', '5 l', 10);
`


function insertDummys() {
  pool.query(tables), (err, res) => {
    console.log(err,res);
  }
}

async function helpInsertBeverage(obj) {
  try {
    await pool.query('INSERT INTO product VALUES (\''+obj.obj.product+'\', \''+obj.obj.manufacturer+'\',\''+obj.obj.categories+'\',\''+obj.obj.description+'\',\''+obj.obj.price+'\',\''+obj.obj.abv+'\',\''+obj.obj.volume+'\',\''+ parseInt(obj.obj.rating)+'\');')
   } catch (err) {
    console.error(err);
  }
}

async function insertBeverage(obj) {

  await helpInsertBeverage(obj);
}

async function helpInsertManufacturer(obj) {
  try {
    await pool.query('INSERT INTO manufacturer VALUES (\''+obj.obj.manufacturer+'\', \''+obj.obj.country+'\', \''+obj.obj.adress+'\');'), (err, res) => {
      console.log(err,res);
    }

  } catch (err) {
    console.error(err);
  }
}

async function insertManufacturer(obj) {
  await helpInsertManufacturer(obj);
}

async function insertCategory(obj) {
  try {
    pool.query('INSERT INTO category VALUES (\''+obj.obj.category+'\', \''+obj.obj.category_desc+'\');'), (err, res) => {
      console.log(err,res);
    }
  } catch (err) {
    console.error(err);
  }
}

async function helpForManufacturers()  {
  try {
    const res = await pool.query('SELECT * FROM manufacturer;');
    return res.rows;
  } catch (err) {
    console.error("error", err.stack)
  }
}
async function getAllManufacturers () {
  var result = await helpForManufacturers();
  return result;
}

async function helpForCategories()  {
  try {
    const res = await pool.query('SELECT * FROM category;');
    return res.rows;
  } catch (err) {
    console.error("error", err.stack)
  }
}
async function getAllCategories () {
  var result = await helpForCategories();
  return result;
}

async function helpForLogger()  {
  try {
    const res = await pool.query('SELECT * FROM logger;');
    return res.rows;
  } catch (err) {
    console.error("error", err.stack)
  }
}
async function getLogger () {
  var result = await helpForLogger();
  return result;
}   

async function helpForProducts()  {
  try {
    const res = await pool.query('SELECT * FROM product;');
    return res.rows;
  } catch (err) {
    console.error("error", err.stack)
  }
}
async function getAllProducts () {
  var result = await helpForProducts();
  return result;
}

async function helpUpdateBeverage(obj) {

  await pool.query(`
  UPDATE product
  SET product = \'${obj.obj.product}\',
      manufacturer = \'${obj.obj.manufacturer}\',
      category = \'${obj.obj.categories}\',
      product_description = \'${obj.obj.description}\',
      product_price = \'${obj.obj.price}\',
      product_abv = \'${obj.obj.abv}\',
      product_volume = \'${obj.obj.volume}\',
      rating = \'${obj.obj.rating}\'
  WHERE \'${obj.obj.oldProduct}\' = product;
  `)
}

async function updateBeverage(obj) {
  await helpUpdateBeverage(obj);
}

async function helpUpdateMf(obj) {
  await pool.query(`
  UPDATE manufacturer
  SET manufacturer = \'${obj.obj.newManufacturer}\',
      country = \'${obj.obj.country}\',
      adress = \'${obj.obj.adress}\'
  WHERE \'${obj.obj.oldManufacturer}\' = manufacturer;
  `)
}

async function updateMf(obj) {
  await helpUpdateMf(obj);
}

async function helpDeleteBev(obj) {
  await pool.query(`
  DELETE FROM product
  WHERE product = \'${obj.obj.product}\';
  `)
}

async function deleteBev(obj) {
  await helpDeleteBev(obj); 
}


async function helpDeleteMF(obj){
  await pool.query(`
  DELETE FROM manufacturer
  WHERE manufacturer = \'${obj.obj.manufacturer}\';
  `)
}

async function deleteMF(obj) {
  await helpDeleteMF(obj);
}

async function deleteCat(obj) {
  await helpDeleteCat(obj);
}
async function helpDeleteCat(obj){
  await pool.query(`
  DELETE FROM category
  WHERE category = \'${obj.obj.category}\';
  `)
}

async function helpUpdateCat(obj) {
  await pool.query(`
  UPDATE category
  SET category = \'${obj.obj.newCat}\',
      category_description = \'${obj.obj.newDesc}\'
  WHERE \'${obj.obj.oldCat}\' = category;
  `)
}

async function updateCat(obj) {
  await helpUpdateCat(obj);
}

module.exports = { 
  updateCat,
  updateMf,
  deleteBev,
  deleteCat,
  deleteMF,
  updateBeverage,
  getAllProducts,
  insertBeverage,
  insertDummys,
  insertCategory,
  insertManufacturer,
  getAllManufacturers,
  getAllCategories,
  getLogger
};
