#[cfg(test)]

#[test]
fn it_works() {
    assert_eq!(2 + 2, 4);
}


#[test]
fn import_test(){
    assert_eq!(rules::adds_hopefully(1,1), 2)
}
