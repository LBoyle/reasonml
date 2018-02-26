[%bs.raw {|require('./app.css')|}];

open Json.Decode;

let str = ReasonReact.stringToElement;

type link = {
  rel: string,
  uri: string
};

type state = {links: list(link)};

type responseType = {state};

type dataType = {data: responseType};

type action =
  | MyNoUpdate
  | GetLinks(state)
  | AddLink(link);

/* produced an error if this line was elsewhere!?!? */
/* I had mine above my type declarations */
let component = ReasonReact.reducerComponent("App");

let parseListItem = res => {
  rel: field("rel", string, res),
  uri: field("uri", string, res)
};

let parseJsonLinks = res => {links: array(parseListItem)};

/* let newItem = (newVal, newId) => {someVal: newVal, id: newId}; */
let parseJsonResponse = res => {data: field("data", parseJsonLinks, res)};

let fetchLinks = () =>
  Js.Promise.(
    Bs_fetch.fetch("https://www.speedrun.com/api/v1")
    |> then_(Bs_fetch.Response.text)
    |> then_(jsonText => resolve(parseJsonLinks(Js.Json.parseExn(jsonText))))
  );

let make = _children => {
  ...component,
  initialState: () => {links: []},
  /* the types are optional the switch statement is not */
  reducer: (action: action, state: state) =>
    switch action {
    | MyNoUpdate => ReasonReact.NoUpdate
    | GetLinks(data) => ReasonReact.Update({links: data.links})
    | AddLink(link) => ReasonReact.Update({links: [link, ...state.links]})
    },
  didMount: self => {
    let handleLinksLoaded = self.reduce(initLinks => GetLinks(initLinks));
    fetchLinks()
    |> Js.Promise.then_(links => {
         handleLinksLoaded(links);
         Js.Promise.resolve();
       })
    |> ignore;
    ReasonReact.NoUpdate;
  },
  render: ({state: {links}}) =>
    <div className="App">
      <div className="App-header"> <h2> (str("Title")) </h2> </div>
      <ul>
        (
          List.map(link => <li key=link.rel> (str(link.uri)) </li>, links)
          |> Array.of_list
          |> ReasonReact.arrayToElement
        )
      </ul>
    </div>
};
/*
 <button
   onClick=(reduce(_e => AddLink(newItem("Hello again", numItems + 1))))>
   (str("Click"))
 </button>
 */
