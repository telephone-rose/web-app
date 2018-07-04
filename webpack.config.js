const path = require("path");
const HtmlWebpackPlugin = require("html-webpack-plugin");
const webpack = require("webpack");
const WebpackCdnPlugin = require("webpack-cdn-plugin");

const outputDir = path.join(__dirname, "build/");

const NODE_ENV = process.env.NODE_ENV || "production";

module.exports = {
  devtool: NODE_ENV === "production" ? "source-map" : "eval",
  entry: ["./lib/es6/src/Index.bs.js"],
  output: {
    path: outputDir,
    filename: "[name].[chunkhash].js",
  },
  optimization: {
    splitChunks: {
      chunks: "all",
    },
  },
  plugins: [
    new HtmlWebpackPlugin({
      template: "src/index.html",
    }),
    new WebpackCdnPlugin({
      modules: {
        react: [
          {
            name: "react",
            var: "React",
            path: `umd/react.${
              NODE_ENV === "production" ? "production.min" : "development"
            }.js`,
          },
          {
            name: "react-dom",
            var: "ReactDOM",
            path: `umd/react-dom.${
              NODE_ENV === "production" ? "production.min" : "development"
            }.js`,
          },
        ],
      },
    }),
  ],
};
