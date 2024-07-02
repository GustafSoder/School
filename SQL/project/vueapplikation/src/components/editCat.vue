<template>
   <div class="inner">
    <form id = "test" class = "container" @submit="checkForm">
      <select class="input2" v-model="selectedCategory">
          <option disabled value="">Please select a old Category</option >
          <option v-for="item in category" :value="item" :key="item">{{item.category}}</option>
      </select >

       <label>    
        <input class="input" type="newCategory" placeholder="New Category" v-model="newCategory" />
      </label>
      
      <label>    
        <input class="input" type="newDesc" placeholder="New Description" v-model="newDesc" />
      </label>
      <button class="input2" type="submit">Update Category</button >
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
      if (this.newCategory && this.selectedCategory) {
        const obj = {
            oldCat: this.selectedCategory.category,
            newCat: this.newCategory,
            newDesc: this.newDesc,
        }
        api.put('/updatecat', {
          obj
        })
        window.location.reload()
      }
      else {
        this.errors = [];

        if (!this.newCategory) {
          this.errors.push('New category required.');
        }
        if (!this.selectedManufacturer) {
          this.errors.push('Category to edit requiered.');
        }
        e.preventDefault();
      }
    },
  },
  data() {
    return {
        errors: [],
        newDesc: "",
        newCategory: "",
        selectedCategory: "",
        category: "",
        oldCat: "",
       
    };
  },
  mounted() {
  axios
    .get('http://localhost:8080/getcategories')
    .then(response => this.category=response.data);
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
