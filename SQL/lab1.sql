/*
   Fill in the queries between the "SELECT QUERY#" and "SELECT END",
   see 'Query 0' for a sample answer.
   If there are any uncertainties contact Oliver and Oscar.

   Structure your queries in a readable format, you do not have
   to use capitalized keywords - Just be consistent.

   You must use joins when filtering, hard coded ids and similar are
   not acceptable. Avoid using natural joins for clarity.
   
   You are not allowed to use temporary tables.
*/

SELECT 'Query 0'; -- Do not remove
-- Select the product names of all products
    SELECT product_name FROM products;

SELECT 'END'; -- Do not remove

SELECT 'Query 1'; -- Do not remove
/* 
    Select the product names and unit prices of all products supplied by 
    supplier 'Tokyo Traders'.
*/
    SELECT product_name, unit_price FROM products AS p JOIN suppliers AS s ON s.supplier_id=p.supplier_id WHERE s.company_name='Tokyo Traders';

SELECT 'END'; -- Do not remove


SELECT 'Query 2'; -- Do not remove
/* 
    Select the product names, category names and units in stock of all
    products supplied by suppliers based in 'Australia', 'Canada' or 'France'.
    NOTE: You are not allowed to use the 'OR' keyword.
*/
    SELECT DISTINCT p.product_name, c.category_name, p.units_in_stock
    FROM products AS p
    JOIN suppliers AS s1
    ON s1.country='Australia'
    JOIN suppliers AS s2
    ON s2.country='Canada'
    JOIN suppliers AS s3
    ON s3.country='France'
    JOIN categories AS c 
    ON c.category_id=p.category_id
    WHERE NOT (p.supplier_id<>s1.supplier_id AND p.supplier_id<>s2.supplier_id AND p.supplier_id<>s3.supplier_id);
    
    
SELECT 'END'; -- Do not remove


SELECT 'Query 3'; -- Do not remove
/* 
   Select the product names and units on order of all products supplied 
   by the country of 'USA', that are not of the category 'Beverages', 
   ordered by the units on order descending.
*/ 
    SELECT DISTINCT product_name, units_on_order 
    FROM products AS p 
    JOIN categories AS c ON c.category_name <> 'Beverages' 
    JOIN suppliers AS s ON s.country ='USA'
    WHERE c.category_id=p.category_id AND s.supplier_id=p.supplier_id 
    ORDER BY units_on_order;
SELECT 'END'; -- Do not remove


SELECT 'Query 4'; -- Do not remove
/*
    Select the first names of the 5 youngest employees at the time of hire,
    ordered by their age at the time in ascending order.
*/
    SELECT first_name
    FROM employees
    ORDER BY hire_date-birth_date
    LIMIT 5;
SELECT 'END'; -- Do not remove


SELECT 'Query 5'; -- Do not remove
/*
    Select the number of orders shipped by each shipper and the shipper company 
    name, grouped by shipper and ordered by the number of orders for 
    each shipper in descending order.
*/
    SELECT count(*) AS "Number of orders", s.company_name
    FROM orders AS o
    JOIN shippers AS s ON o.ship_via=s.shipper_id
    GROUP BY s.company_name
    ORDER BY count(*) DESC;
SELECT 'END'; -- Do not remove


SELECT 'Query 6'; -- Do not remove
/*
    Select the first name of employees that has never had an order made during 
    the months of November or December shipped later than the required date.
*/
    
	SELECT first_name
    FROM employees AS e1
    WHERE NOT EXISTS (SELECT  1
                      FROM orders AS o
                      JOIN employees AS e2 ON e1.first_name=e2.first_name AND (EXTRACT(MONTH FROM o.order_date) = 11 OR EXTRACT(MONTH FROM o.order_date) = 12)
                      AND o.required_date < o.shipped_date AND o.employee_id=e2.employee_id);

SELECT 'END'; -- Do not remove


SELECT 'Query 7'; -- Do not remove
/*
    Select the names of all the products that have been ordered alongside the 
    product 'Ipoh Coffee', i.e. where there exists an order with both the 
    product and ipoh coffee ('Ipoh Coffee' should not be included in the 
    answer). There should be no repeats of product names in your answer.
*/
    SELECT DISTINCT p.product_name
    FROM order_details AS od1 JOIN products AS p ON p.product_id=od1.product_id
    JOIN (SELECT DISTINCT order_id
                FROM order_details AS od1 JOIN products AS p1 ON p1.product_name='Ipoh Coffee' AND od1.product_id=p1.product_id) sq ON sq.order_id=od1.order_id
    WHERE NOT p.product_name='Ipoh Coffee';
SELECT 'END'; -- Do not remove


SELECT 'Query 8'; -- Do not remove
/*
    Select the first and last names of managers as a single column separated by a space,
    the first and last name of their top employee as a single column separated by a space
    in terms of sales (in unit price, accounting for potential discount) 
    during the year 1996, along with the total revenue of the sales of the employee rounded
    down to the nearest integer, ordered by the total revenue. 
    
    Hint: Discount is given as a percentage. '0.2' means that the customer got a 20% discount
          on the purchase.
    Hint: Use `reports_to`

    The expected column format of the output is (header names of columns are not important):
    |first_name_manager last_name_manager|first_name_employee last_name_employee|revenue_by_employee|
    |-----------------------------------------------------------------------------------------------|
    | Magnus Anderson                    | Emma Employee                        | 300               |
    | Alice Svensson                     | Torstein Employed                    | 200               |
*/
    SELECT DISTINCT ON (m.first_name, m.last_name) concat(m.first_name, ' ', m.last_name) AS manager, concat(e.first_name, ' ', e.last_name) AS "employees", FLOOR(SUM(od.quantity*(od.unit_price-(od.unit_price*od.discount)))) AS "revenue"
    FROM employees AS e
    RIGHT OUTER JOIN employees AS m ON m.employee_id = e.reports_to
    JOIN orders AS o ON e.employee_id=o.employee_id AND EXTRACT(YEAR FROM o.order_date) = 1996
    JOIN order_details AS od ON o.order_id=od.order_id 
    GROUP BY m.first_name, m.last_name, employees
    ORDER BY m.first_name, m.last_name, revenue DESC, employees;
SELECT 'END'; -- Do not remove


 
SELECT 'Query 9'; -- Do not remove
/*
    Select the company names of the customers that have made at least one order
    handled by each of the employees. I.E. all employees has been responsible
    for at least one order made by the customer.
*/
    SELECT c.company_name
    FROM orders as o
    JOIN customers AS c ON c.customer_id=o.customer_id
    JOIN employees AS e ON o.employee_id=e.employee_id
    NATURAL JOIN (SELECT count(*)
    FROM employees) AS e2
    GROUP BY c.company_name, e2.count
    HAVING e2.count=count(DISTINCT e.employee_id);
SELECT 'END'; -- Do not remove


SELECT 'Query 10'; -- Do not remove
/*
    Two products are related if they exist in an order together. Two products are also related if they
    have been on an order with a product that 'connects them'.

    Example:
    In the example below, apples and pears are related as they exist on the same order. Apples and
    cheese are also related, as one of the related products of apples are related to cheese. 
    The sets of related products for orders below are thus as follows:

    Note: Orders and producs below are part of your database state.

    Orders:
        Order 1:
            Apples
            Pears
        Order 2:
            Pears
            Cheese
        Order 3:
            Soda
            Candy
        Order 4:
            Candy
            Sugar
        Order 5:
            Gingerbread
            Sugar

    Related products:
        Apples -> (Pears, Cheese)
            Apples -> Pears
                Pears -> Cheese

        Pears  -> (Apples, Cheese)
            Pears -> Apples
            Pears -> Cheese

        Cheese -> (Pears, Apples)
            Cheese -> Pears
                Pears -> Apples

        Candy -> (Sugar, Soda, Gingerbread)
            Candy -> Sugar
                Sugar -> Gingerbread
            Candy -> Soda
        
        Soda -> (Candy, Sugar, Gingerbread)
            Soda -> Candy
                Candy -> Sugar
                    Sugar -> Gingerbread



    Question:
        Create a query that returns the product names of all related
        products of an input product name based on orders,
        ordered by product names ascending. The base product should not be included.

        Call your query with the base product 'Sugar'.
    
        Note: You must use recursion.
 */

    WITH RECURSIVE baseproduct AS (
        SELECT 
            p.product_name,
            o.order_id,
            p.product_id
        FROM
            products AS p
            INNER JOIN order_details AS o ON o.product_id=p.product_id
        WHERE
            p.product_name='Sugar'
        UNION
            SELECT
                p1.product_name,
                o1.order_id,
                p1.product_id
            FROM
                products AS p1
                INNER JOIN order_details AS o1 ON o1.product_id=p1.product_id
                JOIN baseproduct AS bp ON (bp.order_id=o1.order_id OR bp.product_id=p1.product_id)

    ) SELECT DISTINCT product_name
    FROM baseproduct
    WHERE product_name<>'Sugar';
    
SELECT 'END'; -- Do not remove
