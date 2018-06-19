const path = require("path");
const HtmlWebpackPlugin = require("html-webpack-plugin");
const webpack = require("webpack");

const outputDir = path.join(__dirname, "build/");

module.exports = {
  entry: "./lib/es6/src/Index.bs.js",
  output: {
    path: outputDir,
    filename: "[name].[chunkhash].js"
  },
  optimization: {
    splitChunks: {
      chunks: "all"
    }
  },
  plugins: [
    new HtmlWebpackPlugin({
      template: "src/index.html"
    })
  ]
};
