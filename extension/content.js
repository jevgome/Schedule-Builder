
MutationObserver = window.MutationObserver;

var observer = new MutationObserver(function(mutations, observer) {
    // fired when a mutation occurs
    // console.log(mutations, observer);
    // ...
    // var values = document.querySelectorAll('a')
    var values = document.querySelectorAll(
        '[id*="UC_CLASS_G_VW_"], [id*="CLASS_LINK"], [id*="UC_DERIVED_GST_UNITS_RANGE"], [id*="CAMPUS_CLMN"], [id*="INSTRUCT_MODE_DESCR"], [id*="HRS_DAYS_LOC_CLMN"], [id*="UC_DERIVED_GST_DESCR50"], [id*="UC_DERIVED_GST_MAX_ATTENDEE"], [id*="UC_DERIVED_GST_HTMLAREA1"], [id*="UC_DERIVED_GST_WAIT_TOT"], [id*="UC_DERIVED_GST_SSR_INSTR_LONG"]'
    )
    if(values.length == 0) {
        // console.log("The table is not here.")
    } else {
        for(let i = 0; i < values.length; i++) {
            console.log("%s", values[i].innerText)
        }
        observer.disconnect();
    }
});

// define what element should be observed by the observer
// and what types of mutations trigger the callback
// var id = document.querySelector('[id*="UC_CLASS_G_VW_"]')
function obs_page() {
    observer.observe(document, {
        subtree: true,
        attributes: true
        //...
      });
}
obs_page();