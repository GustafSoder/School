<template>
   <div class = "inner">
    <form id = "test" class = "container" @submit="checkForm">
        <label>
            <input class="input" type="category" placeholder = "Category" v-model="categoryname" />
        </label>
        <label>
            <input class="input" type="category_description" placeholder = "Category description" v-model="categoryDesc" />
        </label>
        

      <button class="input2" type="submit">Add Category</button >
      <p v-if="errors.length">
        <b>Please correct the following error(s):</b>
          <ul>
            <li v-for="error in errors" :value="error" :key="error">{{ error }}</li>
          </ul>
      </p>
    </form>
   </div>
</template>

<script>
import axios from 'axios';
const api = axios.create({baseURL: 'http://localhost:8080'})
export default {
  methods:{
    checkForm: function (e) {
      if (this.categoryname) {
        const obj = {
          category: this.categoryname,
          category_description: this.categoryDesc,
        }
        api.put('/addcat', {
          obj
        })
        window.location.reload()
      }
      else {
        this.errors = [];

        if (!this.categoryname) {
          this.errors.push('Category required.');
        }
        e.preventDefault();
      }
    },
  },
  data() {
    return {
        errors: [],
        categoryname: "",
        categoryDesc: "",
       
    };
  },
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



