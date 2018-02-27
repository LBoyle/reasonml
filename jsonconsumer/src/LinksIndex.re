[%bs.raw {|require('./app.css')|}];

open Json.Decode;
/* open Types; */

let str = ReasonReact.stringToElement;

type stateType = {links: list(Types.link)};

type dataType = {data: stateType};

type action =
  | NoUpdate
  | GetLinks(stateType)
  | AddLink(Types.link);

let component = ReasonReact.reducerComponent("LinksIndex");

let parseListItem = (res): Types.link => {
  rel: field("rel", string, res),
  uri: field("uri", string, res)
};

let parseJsonLinks = (res: Js.Json.t): stateType => {links: field("links", list(parseListItem), res)};

let parseJsonResponse = (res: Js.Json.t): dataType => {data: field("data", parseJsonLinks, res)};

let fetchLinks = () =>
  Js.Promise.(
    Fetch.fetch("https://www.speedrun.com/api/v1")
    |> then_(Fetch.Response.text)
    |> then_(jsonText => resolve(parseJsonResponse(Js.Json.parseExn(jsonText))))
    );

let preventDefault = (e, dest: Types.link) => {
  ReactEventRe.Mouse.preventDefault(e);
  ReasonReact.Router.push("/links/" ++ dest.rel);
};

let make = _children => {
  ...component,
  initialState: () => {links: []},
  /* the types are optional the switch statement is not */
  reducer: (action: action, state: stateType) =>
    switch action {
    | NoUpdate => ReasonReact.NoUpdate
    | GetLinks(data) => ReasonReact.Update({links: [...data.links]})
    | AddLink(link) => ReasonReact.Update({links: [link, ...state.links]})
    },
  didMount: self => {
    let handleLinksLoaded = self.reduce((links: stateType) => GetLinks(links));
    fetchLinks()
    |> Js.Promise.then_((data: dataType) => {
      handleLinksLoaded(data.data);
      Js.Promise.resolve(data.data);
    })
    |> ignore;
    ReasonReact.NoUpdate;
  },
  render: self =>
    <div className="LinksIndex">
      <ul>
      (
        List.map((link: Types.link) => 
          <li key=link.rel>
            <a 
              href="#"
              onClick=((e) => preventDefault(e, link))
              >
              (str(link.rel))
            </a>
          </li>
          , self.state.links)
        |> Array.of_list
        |> ReasonReact.arrayToElement
      )
      </ul>
      
    </div>
};

/* (
          /* List.map(link => <li key=link.rel> (str(link.uri)) </li>, self.state.links) */
          List.map((link: Types.link) => 
            <LinkShow 
              key=link.rel 
              link />
            , self.state.links)
          |> Array.of_list
          |> ReasonReact.arrayToElement
        ) */