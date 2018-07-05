let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();

/* Create an HTTP Link */
let httpLink =
  ApolloLinks.createHttpLink(~uri="https://api.telephone-ro.se/graphql", ());

let makeAuthLink = authToken => {
  let contextHandler = () => {
    "headers": {
      "authorization": "Bearer " ++ authToken,
    },
  };
  ApolloLinks.createContextLink(contextHandler);
};

let makeInstance = authToken => {
  let links =
    switch (authToken) {
    | None => [|httpLink|]
    | Some(authToken) => [|makeAuthLink(authToken), httpLink|]
    };

  ReasonApollo.createApolloClient(
    ~link=ApolloLinks.from(links),
    ~cache=inMemoryCache,
    (),
  );
};
