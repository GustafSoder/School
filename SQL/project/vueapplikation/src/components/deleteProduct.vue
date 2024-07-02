<template>
   <div class="inner">
    <form id = "test" class = "dropdown" @submit="checkForm">
      <label>
        <select class="input2" v-model="selectedProduct">
          <option disabled value="">Please select a product to delete</option >
          <option v-for="item in product" :value="item" :key="item">{{item.product}}</option>
      </select>
      </label>

      <button class="input2" type="submit">Delete Beverage</button>
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
    checkForm: function(e) {
      if (this.selectedProduct.product) {
        const obj = {
          product: this.selectedProduct.product,
        }
        api.delete('/deletebev', {
          data: {
            obj
          }
        })
        window.location.reload()
      }
      else {
        this.errors = [];

        if (!this.selectedProduct.product) {
          this.errors.push('Product name required.');
        }
        e.preventDefault();
      }
    },
  },
  data() {
    return {
      errors: [],
      selectedProduct: "",
      product: "",
    };
  },
  mounted() {
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
    justify-content: center;
  }
  .input {
    display: block; 
    width: 250px;
  }
  .input2 {
    display: block; 
    width: 250px;
  }
  .inner { 
    margin: 5px; 
    flex-direction: row;
    display: flex;
  }
</style>



