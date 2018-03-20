

import junit.framework.TestCase;

//You can use this as a skeleton for your 3 different test approach
//It is an optional to use this file, you can generate your own test file(s) to test the target function!
// Again, it is up to you to use this file or not!




public class UrlValidatorTest extends TestCase {


   public UrlValidatorTest(String testName) {
      super(testName);
   }

   
   
   public void testManualTest()
   {
	   /* Various manual test cases: */
/*	   
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	   
	   assertTrue(urlVal.isValid("http://www.google.com"));
	   assertFalse(urlVal.isValid("http:/www.google.com"));
	   */

   }
   
   
   public void testYourFirstPartition()
   {
	 //You can use this function to implement your First Partition testing	   

   }
   
   public void testYourSecondPartition(){
		 //You can use this function to implement your Second Partition testing	   

   }
   //You need to create more test cases for your Partitions if you need to 
   
	public void testIsValid()
	{
		testPair[] test_scheme = {
				new testPair("http://", true),
				new testPair("Htp:/", false),
				new testPair("hptp:/", false),
                new testPair("http/", false),
                new testPair("://", false),
                new testPair("", true)
		};
		int nSchemes = 6;
		
		testPair[] test_authority = {
				new testPair("www.google.com", true),
				new testPair("www.gogle.com", true),
				new testPair("google.com", true),
				new testPair("1a.as", false),
                new testPair(".1.2.2.4", false),
                new testPair("", false)
		};
		int nAuths = 6;
		
		testPair[] test_port = {
				new testPair(":80", true),
				new testPair(":65666", false),
				new testPair("", true),
				new testPair("-12", false)
		};
		int nPorts = 4;
		

		testPair[] test_path = {
				new testPair("/file", true),
				new testPair("/asdf", true),
				new testPair("/..", false)
		};
		int nPaths = 3;
		
		testPair[] test_querie = {
				new testPair("?query=test", true),
				new testPair("?query=test&testb", true)
		};
		int nQueries = 2;
		

		String url;
		boolean expect;
		boolean res;
		
		UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);

		
		for (int i=0; i<nSchemes; i++) {
			for (int j=0; j<nAuths; j++) {
				for (int k=0; k<nPorts; k++) {
					for (int h=0; h<nPaths; h++) {
						for (int w=0; w<nQueries; w++) {
							url = test_scheme[i].item + test_authority[j].item + test_port[k].item + test_path[h].item + test_querie[w].item;
							expect = test_scheme[i].valid & test_authority[j].valid & test_port[k].valid & test_path[h].valid & test_querie[w].valid;
							
							//System.out.printf("%s\t\t===>%s\n", url, expect ? "true" : "false");
							
							/*
							 * NOTE:  Not going to use Junit, as there are too many faults on assert<True/False> causing the programming to hault.
							 *
							
							if (expect == true){
								   assertTrue(urlVal.isValid(url));
							} else {
								   assertFalse(urlVal.isValid(url));
							}
							*/
							
							res = urlVal.isValid(url);
							if (res != expect) {
								System.out.printf("%s\t%s\t%s\n", url, expect ? "true" : "false", res ? "true" : "false");
							}
						}
					}
				}
			}
		}
	}
}
