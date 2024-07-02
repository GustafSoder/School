<template>
  
  <div id = "outerDiv">
    <h1>Lilla Bolaget</h1>
    Products
    <div class = "output">
      <addProduct></addProduct>
      <deleteProduct></deleteProduct>
      <editProduct></editProduct>
    </div>
    Manufacturer
    <div class = "output">
      <addMf></addMf>
      <deleteMf></deleteMf>
      <editMf></editMf>
    </div >
    Category
    <div class = "output">
      <addCat></addCat>
      <deleteCat></deleteCat>
      <editCat></editCat>
    </div>
    <viewProduct></viewProduct>
    <viewMf></viewMf>
    <viewCategories></viewCategories>
    <viewLogger></viewLogger>
  </div>
</template>

<script>
import addProduct from './components/addProduct.vue'
import deleteProduct from './components/deleteProduct.vue'
import editProduct from './components/editProduct.vue'
import addMf from './components/addMf.vue'
import viewProduct from './components/viewProduct.vue'
import addCat from './components/addCat.vue'
import deleteMf from './components/deleteMf.vue'
import deleteCat from './components/deleteCat.vue'
import viewCategories from './components/viewCategories.vue'
import editMf from './components/editMf.vue'
import viewMf from './components/viewMf.vue'
import editCat from './components/editCat.vue'
import viewLogger from './components/viewLogger.vue'
export default {
  methods:{
    getData(){

    }
  },
  data(){
      return {
        input: null,
    }
  },
  components: {
    addProduct, deleteProduct, editProduct, addMf, viewProduct,
    addCat, viewCategories, viewMf, editMf, deleteMf, deleteCat, editCat, viewLogger
  },
}
</script>
  
<style scoped>
  #outerDiv {
    /* background: #000000; */
    display: flex;
    align-items: center;
    flex-direction: column;
  }

  .output {
    /* background: #000000; */
    flex-direction: row;
    display: flex;
    justify-content: space-between;
  }
  
</style>