<template>
   <div class="inner">
    <form id = "test" class = "container" @submit="checkForm">
      <select class="input2" v-model="selectedProduct">
          <option disabled value="">Please select a product</option >
          <option v-for="item in product" :value="item" :key="item">{{item.product}}</option>
      </select >
      <label>
        
        <input class="input" type="newName" placeholder = "New Name" v-model="newProductName" />
      </label>
      <select class="input2" v-model="newSelectedManufacturer">
          <option disabled value="">Please select a new manufacturer</option >
          <option v-for="item in manufacturers" :value="item" :key="item">{{item.manufacturer}}</option>
      </select>

       <select class="input2" v-model="newSelectedCategory">
          
          <option disabled value="">Please select a new category</option >
          <option v-for="item in categories" :value="item" :key="item">{{item.category}}</option>
      </select >
      
      <label>    
        <input class="input" type="newdesc" placeholder="New Description" v-model="newdesc" />
      </label>

       <label>
        <input class="input" type="newprice" placeholder="New Price" v-model="newprice" />
      </label>

      <label>      
        <input class="input" type="newabv" placeholder="New Abv" v-model="newabv" />
      </label>

      <label>
        <input class="input" type="newvolume" placeholder="New Volume" v-model="newvolume" />
      </label>
        
      <label>  
        <input class="input" type="newrating" placeholder="New Rating" v-model="newrating" />
      </label>
      <button class="input2" type="submit">Update Beverage</button  >
      <p v-if="errors.length">
        <b>Please correct the following error(s):</b>
          <ul>
            <li v-for="error in errors" :value="error" :key="error">{{ error }}</li>
          </ul>
      </p>
    </form>
   </div >
</template>

<script>
import axios from 'axios';
const api = axios.create({baseURL: 'http://localhost:8080'})
export default {
  methods: {
    checkForm: function (e) {
      if (this.newProductName && this.newProductName && this.newSelectedManufacturer && this.newSelectedCategory) {
        const obj = {
          oldProduct: this.selectedProduct.product,
          product: this.newProductName,
          manufacturer: this.newSelectedManufacturer.manufacturer,
          categories: this.newSelectedCategory.category,
          description: this.newdesc,
          price: this.newprice,
          abv: this.newabv,
          volume: this.newvolume,
          rating: this.newrating
        }
        api.put('/updatebev', {
          obj
        })
        window.location.reload()
      }
      else {
        this.errors = [];

        if (!this.newProductName) {
          this.errors.push('New product name required.');
        }
        if (!this.newSelectedManufacturer) {
          this.errors.push('Manufacturer requiered.');
        }
        if (!this.newSelectedCategory) {
          this.errors.push('Category required.');
        }
        if (!this.selectedProduct.product) {
          this.errors.push('Old product name required.');
        }
        e.preventDefault();
      }
    },
  },
  data() {
    return {
      errors: [],
      product: "",
      productName: "",
      newProductName: "",
      manufacturers: "",
      newmanufacturers:"",
      categories: "",
      newcategories: "",
      newdesc: "",
      newprice: "",
      newabv: "",
      newvolume: "",
      newrating: null,
      selectedProduct: "",
      selectedManufacturer: "",
      selectedCategory: "",
      newSelectedManufacturer: "",
      newSelectedCategory: ""
    };
  },
  mounted() {
  axios
    .get('http://localhost:8080/getmf')
    .then(response => this.manufacturers=response.data);
  axios
    .get('http://localhost:8080/getcategories')
    .then(response => this.categories=response.data);
  axios
    .get('http://localhost:8080/getproducts')
    .then(response => this.product=response.data);
  }
}

</script>

<style>

#container {
  width: 800px;
  font-family: Helvetica, sans-serif;
  
}

 #test {
    /* background: #000000; */
    flex-direction: column;
    display: flex;
    justify-content: center;
  }
  .input {
    display: block; 
    width: 200px;
  }
  .input2 {
    display: block; 
    width: 207px;
  }
  .inner { 
    margin: 5px; 
    flex-direction: row;
    display: flex;
  }
</style>
