<template>
   <div class = "inner">
    <form id = "test" class = "inner" @submit="checkForm">
      <label>
        <input class="input" type="pName" placeholder = "Name" v-model="productName" />
      </label>

      <select class = "input2" v-model="selectedManufacturer">
          <option disabled value="">Please select a Manufacturer</option >
          <option v-for="item in manufacturers" :value="item" :key="item">{{item.manufacturer}}</option>
      </select >

      <select class = "input2"  v-model="selectedCategory">
          <option disabled value="">Please select a Category</option  >
          <option v-for="item in categories" :value="item" :key="item">{{item.category}}</option >
      </select  >
      
      <label >  
        <input class = "input" type="description" placeholder="Description" v-model="description" />
      </label >
      
      <label >
        <input class = "input" type="price" placeholder="Price" v-model="price" />
      </label >
      
      <label >    
        <input class = "input" type="abv" placeholder="Abv" v-model="abv" />
      </label >

      <label >   
        <input class = "input" type="volume" placeholder="Volume" v-model="volume" />
      </label >

      <label >  
        <input class = "input" type="rating" placeholder="Rating" v-model="rating" >
      </label>

      <button class = "input2" type="submit">Add Beverage</button >
      <p v-if="errors.length">
        <b>Please correct the following error(s):</b>
          <ul>
            <li v-for="error in errors" :value="error" :key="error">{{ error }}</li>
          </ul>
      </p>
    </form >
   </div  >
</template>

<script>
import axios from 'axios';
const api = axios.create({baseURL: 'http://localhost:8080'})
export default {
  methods:{
    checkForm: function (e) {
      if (this.productName && this.selectedManufacturer.manufacturer
          && this.selectedCategory.category) {
        const obj = {
          product: this.productName,
          manufacturer: this.selectedManufacturer.manufacturer,
          categories: this.selectedCategory.category,
          description: this.description,
          price: this.price,
          abv: this.abv,
          volume: this.volume,
          rating: this.rating
        }
        api.put('/addbev', {
          obj
        })
        window.location.reload()
      }
      else {
        this.errors = [];

        if (!this.productName) {
          this.errors.push('Product name required.');
        }
        if (!this.selectedManufacturer.manufacturer) {
          this.errors.push('Manufacturer required');
        }
        if (!this.selectedCategory.category) {
          this.errors.push('Category required');
        }
        e.preventDefault();
      }
    },
  },
  data() {
    return {
      errors: [],
      productName: "",
      manufacturers: "",
      categories: "",
      desc: "",
      price: "",
      abv: "",
      volume: "",
      rating: null,
      selectedManufacturer: "",
      selectedCategory: ""
    };
  },
  mounted() {
  axios
    .get('http://localhost:8080/getmf')
    .then(response => this.manufacturers=response.data);
  axios
    .get('http://localhost:8080/getcategories')
    .then(response => this.categories=response.data);
  }
}
</script>

<style>

#container {
  width: 800px;
  font-family: Helvetica, sans-serif;
}
h1 {
  text-align: center
}
img {
  text-align: center
}
.submitted {
  color: #4fc08d;
}


 #test {
    /* background: #000000; */
    flex-direction: column;
    display: flex;
    align-items: center;
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



