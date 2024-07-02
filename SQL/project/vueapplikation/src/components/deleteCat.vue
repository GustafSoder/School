<template>
   <div class="inner">
    <form id = "test" class = "container" @submit="checkForm">
      <label>
        <select class="input2" v-model="selectedCategory">
          <option disabled value="">Please select a Category to delete</option >
          <option v-for="item in category" :value="item" :key="item">{{item.category}}</option>
      </select >
      </label>

      <button class="input2" type="submit">Delete Category</button>
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
      if (!this.products.some(data => data.category === this.selectedCategory.category)) {
        if (this.selectedCategory.category) {
          const obj = {
            category: this.selectedCategory.category,
          }
          api.delete('/deletecat', {
            data: {
              obj
            }
          })
          window.location.reload()
        }
        else {
          this.errors = [];

          if (!this.selectedCategory.category) {
            this.errors.push('Category name required.');
          }
          e.preventDefault();
        }
      }
      else {
        this.errors.push('Category has a beverage in the database,\n delete them first to delete the category');
        e.preventDefault();
      }
    },
  },
  data() {
    return {
      errors: [],
      selectedCategory: "",
      category: "",
      products: "",
    };
  },
  mounted() {
  axios
    .get('http://localhost:8080/getcategories')
    .then(response => this.category=response.data);
  api
    .get('/getproducts')
    .then(response => this.products=response.data);
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



