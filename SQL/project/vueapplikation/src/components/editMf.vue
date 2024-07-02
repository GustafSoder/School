<template>
   <div class="inner">
    <form id = "test" class = "container" @submit="checkForm">
      <select class="input2" v-model="selectedManufacturer">
          <option disabled value="">Please select a old manufacturer</option >
          <option v-for="item in manufacturers" :value="item" :key="item">{{item.manufacturer}}</option>
      </select>

       <label>    
        <input class="input" type="newManufacturer" placeholder="New Manufacturer" v-model="newManufacturer" />
      </label>
      
      <label>    
        <input class="input" type="newCountry" placeholder="New Country" v-model="newCountry" />
      </label>

       <label>
        <input class="input" type="newAdress" placeholder="New adress" v-model="newAdress" />
      </label>
      <button class="input2" type="submit">Update Beverage</button >
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
      if (this.newManufacturer && this.selectedManufacturer) {
        const obj = {
            oldManufacturer: this.selectedManufacturer.manufacturer,
            newManufacturer: this.newManufacturer,
            country: this.newCountry,
            adress: this.newAdress
        }
        api.put('/updatemf', {
          obj
        })
        window.location.reload()
      }
      else {
        this.errors = [];

        if (!this.newManufacturer) {
          this.errors.push('New manufacturer required.');
        }
        if (!this.selectedManufacturer) {
          this.errors.push('Manufacturer to edit requiered.');
        }
        e.preventDefault();
      }
    },
  },
  data() {
    return {
        errors: [],
        newCountry: "",
        country: "",
        newAdress: "",
        adress: "",
        manufacturers: "",
        newManufacturer: "",
        oldManufacturer: "",
        selectedManufacturer: "",
    };
  },
  mounted() {
  axios
    .get('http://localhost:8080/getmf')
    .then(response => this.manufacturers=response.data);
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
