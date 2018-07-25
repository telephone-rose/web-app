type nlpAttributes = {quoteStructure: array(array(string))};

type fact = {
  message: string,
  nlpAttributes,
};

let _factDecoder = jsonFact =>
  Json.Decode.{
    message: jsonFact |> field("message", string),
    nlpAttributes: {
      quoteStructure:
        jsonFact
        |> at(["nlp_attributes", "quote_structure"], array(array(string))),
    },
  };

let random = () =>
  Js.Promise.(
    Fetch.fetch("https://api.whatdoestrumpthink.com/api/v1/quotes/random")
    |> then_(response => response |> Fetch.Response.json)
    |> then_(jsonResponse => jsonResponse |> _factDecoder |> resolve)
  );
