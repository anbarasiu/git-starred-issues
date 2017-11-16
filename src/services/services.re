let env = "";

let getStarredRepos = () : Js.Promise.t(Bs_fetch.response) => 
    env === "DEV" ? 
    Bs_fetch.fetch("data.json") :
    Bs_fetch.fetch("https://api.github.com/users/anbarasiu/starred");