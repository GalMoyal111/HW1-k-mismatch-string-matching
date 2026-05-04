#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <unordered_map>
#include <iterator>
#include <unordered_set>
#include <sstream>
#include <chrono>
#include <cmath>

using namespace std;
using namespace std::chrono;

struct WordPositions {
    string word;
    vector<int> positions;
};


struct WordPositionsSet {
    string word;
    unordered_set<int> positions;
};


void createRandomText(const vector<string>& words, double sizeInMB, const string& filename, size_t wordLength);
string makeRandomUppercaseString(size_t length);
string makeWordWithErrors(const string& word, int K, mt19937& gen);
vector<string> generateSetOfWordsWithErrors(const vector<string>& queries, int K);
void generateText(double sizeInMB, const string& filename, int m, int k, int s, const vector<string>& queries);
bool matchesWithErrors(const string& word, const string& segment, int k);
vector<int> findWordWithErrors(const string& text, const string& word, int k);
vector<WordPositions> findSetWordsNaiveSearch(const string& text, const vector<string>& words, int k);
vector<string> getBinaryCombinations(int m, int k);
bool configMatchescombinations(const string& combo, const string& config);
vector<string> buildConfigurationsBasic(int M, int m, int k, const vector<string>& combinations, vector<string>& configurations);
unordered_map<string, vector<int>> buildMatches(const string& text, const vector<string>& configurations, bool returnMap, vector<string>* outList = nullptr);
vector<WordPositionsSet> findSetWods(const vector<string>& configurations, const vector<string>& queries, const string& text, const unordered_map<string, vector<int>>& results, int m, int M, int k);
vector<string> buildConfigurations(int M, const vector<string>& combinations);
vector<string> buildConfigurationsWeightedConfiguration(int M, int m, int k);
vector < vector <string>> CreateVecOfSets(int M, int m, int k);
vector <WordPositionsSet> fideWordsWhithPositions(const vector<vector<string>>& vecOfSet, const vector<string>& queries, const string& text, const unordered_map<string, vector<int>>& results, int M, int m, int k);
vector<string> buildPosMatches(const string& text, const vector<string>& configurations, int pos);
unordered_set<string> buildAllowedPatternsFromQueries(const vector<string>& configurations,const vector<string>& queries);
unordered_map<string, vector<int>> buildMatchesFiltered(const string& text,const vector<string>& configurations,const unordered_set<string>& allowedPatterns);






int main()
{

    int m = 18 + 1 ; //querie size
    int k = static_cast<int>(ceil(0.4 * m)); //number of errors
	int s = 41; //set size
	double sizeInMB = 100;//text size in MB
    string filename = "text.txt";
    vector<string> queries = { "FGAUWQGVCPTSTMJVLPT","UVWBVSAZBGNVBXNRRBZ","EWKRHFERYTYLWTGDMBJ","MDSSANCFBGUIBLPYYHT","LZEEWWABQISVGPUYZWF","TECAVVYHQXHJQCQHZBX","JTXUBICXVXDMLLRHKBH","YYXPZOWPZAQJNIJZDQK","SDVDANMMVAMLXMFFCLL","OOSURHXQQGAIEZIXOPJ","OKWYUXHBMBWPABVSPHS","ONAEUFYZNPVACDOWZCX","OXIODPEPJAFUTULLKMK","VNUJJXNKRGQZRTKUXJP","QIIVQEQRBVFPXZNIGCX","ZRRSZEPFDWCWEUQLBBB","WIWBOBYJPONZNVSXCQG","PIWUXMEHYGYGBNNANSD","IZPKARSVFUBCJOZBKKY","YVCATKZQCDDVIITANFN","ERCWIFCDVDQZQKSCSRA","MWFKGKQGFZHEJDMIRNU","GRKMFGXFBJZXSKDIQJP","PGXRVEGGCQOVNVKLMZZ","OYARXJQSKNDAGXTDDQG","HSRXRSMMRJWUIDLUXAH","HLJACPYAJFQBFNBQBBX","IQIHZBQGEOENSKXHKLP","ZTKVETOSVSAYIVWVNOB","HKRGEGSIAKHRYBEJXKN","SYAKUNXFSAMXRVHHUVB","OKPFOKFKUYYGHZSMQVO","YZWGIWOUAYDOZAIYYVY","ADEXNSCRAZLIXJCQPYF","MDIICRKMRFNQTMQTNQX","XPZFOIPRQWGNCFXKBME","QXMJRETBLHBFQRPUGYO","MWDFUEPBIBXAKRKFIIR","XSOXBNEUAMVTYYUILOW","ZELKACTMUOJOAVDSCJX","HAGUSWVGLAFGWTMJFIL" };
    
    int M = 3; // number of ones in configuration 
    int generateTextFlag = 0;
	int algorithm = 1;
    int testFlag = 1;

	// for testing
    vector <WordPositionsSet> test;


	//----------------------------------------- Generate text file -----------------------------------------//
    if (generateTextFlag == 1) {

        //generate text file
        generateText(sizeInMB, "text.txt", m, k, s, queries);
	}


	//----------------------------------------- Select algorithm -----------------------------------------//
	// Naive search
    if (algorithm == 0) {
        
        // read the entire text from the file
        string text;
        {
            // Open the file in binary mode
            ifstream in(filename, ios::in | ios::binary);
            if (!in) {
                cerr << "Error: cannot open file " << filename << "\n";
                return {};
            }
            text.assign(istreambuf_iterator<char>(in), istreambuf_iterator<char>());
        }   
        
        // Start timing
        auto start = high_resolution_clock::now();

        cout << "Naive Search Results \n";
        vector<WordPositions> locations = findSetWordsNaiveSearch(text, queries, k);

        // End timing
        auto end = high_resolution_clock::now();
        duration<double> elapsed = end - start;
        
        // Open results file in append mode
        ofstream results("results.txt", ios::app);
        if (!results) {
            cerr << "Error: cannot open results.txt for writing\n";
            return {};
        }

        // Separator between runs
        results << "\n";
        results << "=====================================\n";
        results << "       Naive Algorithm Run Results\n";
        results << "=====================================\n\n";
        results << "Text size: " << sizeInMB << "M\n";
        // Write elapsed time
        results << "Elapsed time: " << elapsed.count() << " seconds\n\n";
        results << "Theoretical runtime (Naive): O(t)\n";

        results << "\n"; // empty line between runs
      
        results.close();   
    
        // for testing
        test.clear();
        for (const auto& wp : locations) {
            WordPositionsSet s;
            s.word = wp.word;         
            s.positions.insert(wp.positions.begin(), wp.positions.end());
            test.push_back(std::move(s));
        }

    }



	// Basic Configuration search
    else if (algorithm == 1) {
        
        cout << "Basic Configuration Algorithm\n";

        // Get all binary combinations of length m with k zeros
        vector<string> combinations = getBinaryCombinations(m, k);
        int combiSize = static_cast<int>(combinations.size());

        vector<string> configurations = buildConfigurations(M, combinations);
        int initialConfigSize = static_cast<int>(configurations.size());

        vector<string> configurationsBasic = buildConfigurationsBasic(M, m, k, combinations, configurations);

        unordered_set<string> allowedPatterns = buildAllowedPatternsFromQueries(configurationsBasic, queries);

        // read the entire text from the file
        string text;
        {
            ifstream in(filename, ios::in | ios::binary);
            if (!in) {
                cerr << "Error: cannot open file " << filename << "\n";
                return {};
            }
            text.assign(istreambuf_iterator<char>(in), istreambuf_iterator<char>());
        }
        cout << "text loaded\n";

        // build matches only for allowed patterns 
        unordered_map<string, vector<int>> resultsBasic = buildMatchesFiltered(text, configurationsBasic, allowedPatterns);

        // Start timing
        auto start = high_resolution_clock::now();

        // Find word positions using basic configurations
        vector<WordPositionsSet> locationsBasic = findSetWods(configurationsBasic, queries, text, resultsBasic, m, M, k);

        // End timing
        auto end = high_resolution_clock::now();
        duration<double> elapsed = end - start;

        ofstream results("results.txt", ios::app);
        if (!results) {
            cerr << "Error: cannot open results.txt for writing\n";
            return {};
        }

        results << "\n";
        results << "============================================\n";
        results << "      Basic Configuration Search Results\n";
        results << "============================================\n\n";

		results << "Text size: " << sizeInMB << "M\n";
        results << "Number of combinations: " << combiSize << "\n";
        results << "Number of configurations (before reduction): " << initialConfigSize << "\n";
        results << "Number of configurations (after reduction):  " << configurationsBasic.size() << "\n\n";

        // Print elapsed time
        results << "Elapsed time: " << elapsed.count() << " seconds\n\n";

        const double sigmaSize = 26.0;
        double powSigmaM = std::pow(sigmaSize, M);

        long long sumBasic = 0;
        for (const auto& cfg : configurationsBasic) {
            int len = static_cast<int>(cfg.size());
            sumBasic += (m - len + 1);
        }

        double speedupBasic = 0.0;
        if (sumBasic > 0) {
            speedupBasic = powSigmaM / static_cast<double>(sumBasic);
        }

        results << "Theoretical speedup over naive (Basic MCS): " << speedupBasic << "x\n\n";

        // Print reduced configurations
        results << "MCS of M = " << M << ":\n";
        for (const auto& cfg : configurationsBasic) {
            results << cfg << "\n";
        }

        results << "\n\n";

        results.close();

        cout << "Basic Configuration Search Results:\n";
        for (const WordPositionsSet& wp : locationsBasic) {
            cout << "Word: " << wp.word << " Positions: ";
            for (int pos : wp.positions) {
                cout << pos << " ";
            }
            cout << endl;
        }

		// for testing
        test = locationsBasic;

    }



	// Weighted Configuration search
    else if (algorithm == 2) {
        
        // Build weighted configurations
        vector<string> configurationsWeighted = buildConfigurationsWeightedConfiguration(M, m, k);

        unordered_set<string> allowedPatternsWeighted = buildAllowedPatternsFromQueries(configurationsWeighted, queries);

        // read the entire text from the file
        string text;
        {
            // Open the file in binary mode
            ifstream in(filename, ios::in | ios::binary);
            if (!in) {
                cerr << "Error: cannot open file " << filename << "\n";
                return {};
            }
            text.assign(istreambuf_iterator<char>(in), istreambuf_iterator<char>());
        }

        // build all matches from the text based on configurations
        unordered_map<string, vector<int>> resultsWeighted = buildMatchesFiltered(text, configurationsWeighted, allowedPatternsWeighted);
        
        // Start timing
        auto start = high_resolution_clock::now();

        // Find word positions using weighted configurations
        vector<WordPositionsSet> locationsWeighted = findSetWods(configurationsWeighted, queries, text, resultsWeighted, m, M, k);

        // End timing
        auto end = high_resolution_clock::now();
        duration<double> elapsed = end - start;

        // Open results file in append mode
        ofstream results("results.txt", ios::app);
        if (!results) {
            cerr << "Error: cannot open results.txt for writing\n";
            return {};
        }

        // Separator and header
        results << "\n";
        results << "============================================\n";
        results << "    Weighted Configuration Search Results\n";
        results << "============================================\n\n";
        results << "Text size: " << sizeInMB << "M\n";
        results << "Number of configurations (after reduction):  " << configurationsWeighted.size() << "\n\n";

        // Print elapsed time
        results << "Elapsed time: " << elapsed.count() << " seconds\n\n";

        // --- Theoretical speedup vs naive (Weighted MCS) ---
        const double sigmaSize = 26.0;
        double powSigmaM = std::pow(sigmaSize, M);

        long long sumWeighted = 0;
        for (const auto& cfg : configurationsWeighted) {
            int len = static_cast<int>(cfg.size());
            sumWeighted += (m - len + 1);
        }

        double speedupWeighted = 0.0;
        if (sumWeighted > 0) {
            speedupWeighted = powSigmaM / static_cast<double>(sumWeighted);
        }

        results << "Theoretical speedup over naive (Weighted MCS): "
            << speedupWeighted << "x\n\n";

        // Print reduced configurations
        results << "MCS of M = " << M << ":\n";

        for (const auto& cfg : configurationsWeighted) {
            results << cfg << "\n";
        }

        results << "\n\n"; // separation for readability

        results.close();

		// for testing
        test = locationsWeighted;
    }



	// Positional associated MCS search
    else if (algorithm == 3) {

		// Create vector of sets of configurations
        vector < vector <string>>  vecOfSet = CreateVecOfSets(M, m, k);

        unordered_set<string> allowedPatternsPositional = buildAllowedPatternsFromQueries(vecOfSet[0], queries);

        // read the entire text from the file
        string text;
        {
            // Open the file in binary mode
            ifstream in(filename, ios::in | ios::binary);
            if (!in) {
                cerr << "Error: cannot open file " << filename << "\n";
                return {};
            }
            text.assign(istreambuf_iterator<char>(in), istreambuf_iterator<char>());
        }

        unordered_map<string, vector<int>> resultsSet = buildMatchesFiltered(text, vecOfSet[0], allowedPatternsPositional);

        // Start timing
        auto start = high_resolution_clock::now();

        // Find word positions using positional associated MCS
        vector <WordPositionsSet> locationsPositional = fideWordsWhithPositions(vecOfSet, queries, text, resultsSet, M, m, k);

        // End timing
        auto end = high_resolution_clock::now();
        duration<double> elapsed = end - start;

        // Open results file in append mode
        ofstream results("results.txt", ios::app);
        if (!results) {
            cerr << "Error: cannot open results.txt for writing\n";
            return {};
        }

        // Separator and header
        results << "\n";
        results << "============================================\n";
        results << "    Positional associated MCS Results\n";
        results << "============================================\n\n";

        results << "Text size: " << sizeInMB << "M\n";
        // Print elapsed time
        results << "Elapsed time: " << elapsed.count() << " seconds\n\n";


        // --- Theoretical speedup vs naive (Positional MCS) ---
        const double sigmaSize = 26.0;
        double powSigmaM = std::pow(sigmaSize, M);

        std::size_t totalConfigs = 0;
        for (const auto& cfgSet : vecOfSet) {
            totalConfigs += cfgSet.size();
        }

        double speedupPositional = 0.0;
        if (totalConfigs > 0) {
            speedupPositional = powSigmaM / static_cast<double>(totalConfigs);
        }
        results << "MCS of M = " << M << ":\n";
        results << "Number of MCS sets: " << vecOfSet.size() << "\n";
        results << "Total configurations across all MCS sets: "<< totalConfigs << "\n";
        results << "Theoretical speedup over naive (Positional MCS): "<< speedupPositional << "x\n\n";

   
        results << "Number of MCS sets: " << vecOfSet.size() << "\n\n";

        for (size_t i = 0; i < vecOfSet.size(); ++i) {
            const auto& cfgSet = vecOfSet[i];

            results << "MCS #" << (i + 1)
                << " - number of configurations: " << cfgSet.size() << "\n";

            for (const auto& cfg : cfgSet) {
                results << "    " << cfg << "\n";  
            }

            results << "\n"; 
        }

        results << "\n\n"; // separation for readability
        results.close();

		// for testing
        test = locationsPositional;
        
    }
  

	//---------------------------------------------------------------------------------------------------------//
    // test
    if (testFlag == 1) {
        // --- Read correct positions from positions.txt ---
        ifstream in("positions.txt");

        unordered_set<int> correctPositions;
        string line;

        while (getline(in, line)) {
            // Format: WORD : POSITION
            size_t pos = line.find(':');
            if (pos == string::npos) continue;

            string numStr = line.substr(pos + 1);
            int position = stoi(numStr);
            correctPositions.insert(position);
        }

        in.close();

        // --- Collect all positions found in text ---
        unordered_set<int> foundPositions;

        for (const auto& wp : test) {
            for (int p : wp.positions) {
                foundPositions.insert(p);
            }
        }

        // --- Compare: find missing positions ---
        vector<int> missing;

        for (int p : correctPositions) {
            if (foundPositions.find(p) == foundPositions.end()) {
                missing.push_back(p);
            }
        }

        // --- Print results ---
        cout << "===== Position Verification =====\n";
        cout << "Total correct positions in file: " << correctPositions.size() << "\n";
        cout << "Total found positions: " << foundPositions.size() << "\n\n";

        if (missing.empty()) {
            cout << "All correct positions were found.\n\n";
        }
        else {
            cout << "Missing positions (" << missing.size() << "): ";
            for (int p : missing) cout << p << " ";
            cout << "\n\n";
        }
    }
		
    
    

    return 0;
}



// make query words
// Function to generate a random uppercase string of given length
string makeRandomUppercaseString(size_t length)
{
    string result(length, 'A');
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist('A', 'Z');

    // Fill the string with random uppercase letters
    for (size_t i = 0; i < length; i++)
    {
        result[i] = static_cast<char>(dist(gen));
    }
    return result;
}


// crete text

// Function to create a random text file of specified size with all words included at least once
// words: list of words to include
// sizeInMB: desired size of the text file in megabytes
// filename: name of the output file
// wordLength: length of each word
// The function generates random uppercase letters and inserts each word at a random position
void createRandomText(const vector<string>& words,double sizeInMB,const string& filename,size_t wordLength)
{
    size_t textSize = static_cast<size_t>(sizeInMB * 1024 * 1024);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> letterDist('A', 'Z');
	// Ensure there are words to insert
    size_t n = words.size();
    if (n == 0) {
        cerr << "No words provided.\n";
        return;
    }

    vector<size_t> positions(n);
	size_t segmentSize = textSize / n; // Divide text into n segments

	// Determine random positions for each word
    for (size_t i = 0; i < n; ++i) {
        uniform_int_distribution<size_t> posDist(i * segmentSize,
            (i + 1) * segmentSize - wordLength);
        positions[i] = posDist(gen);
    }
	// Sort placements by position
    vector<pair<size_t, const string*>> placements;
    for (size_t i = 0; i < n; ++i)
        placements.push_back({ positions[i], &words[i] });
    sort(placements.begin(), placements.end(),
        [](const auto& a, const auto& b) { return a.first < b.first; });
	// Create and open the output file
    ofstream out(filename, ios::binary);
    if (!out) {
        cerr << "Error opening file " << filename << endl;
        return;
    }

    ofstream posOut("positions.txt");
    if (!posOut) {
        cerr << "Error creating positions file\n";
        return;
    }

    for (size_t i = 0; i < n; i++) {
        posOut << words[i] << " : " << positions[i] << "\n";
    }
    posOut.close();

    size_t currentPos = 0;
    size_t nextWordIndex = 0;
	// Fill the text with random letters and insert words at designated positions
    while (currentPos < textSize) {
        if (nextWordIndex < placements.size() &&
            currentPos == placements[nextWordIndex].first)
        {
            const string& w = *placements[nextWordIndex].second;
            out.write(w.data(), wordLength);
            currentPos += wordLength;
            nextWordIndex++;
        }
        else {
            char c = static_cast<char>(letterDist(gen));
            out.write(&c, 1);
            currentPos++;
        }
    }

    out.close();
    cout << "File created successfully: " << filename << endl;
}

// Function to introduce up to K random character errors in a word
// word: original word
// K: maximum number of errors to introduce
string makeWordWithErrors(const string& word, int K, mt19937& gen)
{
    string result = word;
    uniform_int_distribution<int> errorDist(0, K); 
    uniform_int_distribution<int> posDist(0, word.size() - 1);
    uniform_int_distribution<int> charDist('A', 'Z');

	// Determine number of errors to introduce
    int numErrors = errorDist(gen);
    for (int i = 0; i < numErrors; i++) {
        int pos = posDist(gen);
        char newChar = static_cast<char>(charDist(gen));
        result[pos] = newChar;
    }

    return result;
}

// Function to generate words with errors for each query 10*s times
// queries: list of original words
// K: maximum number of errors to introduce
// Returns a vector of mutated words with errors
vector<string> generateSetOfWordsWithErrors(const vector<string>& queries, int K)
{
    vector<string> allWords;
    random_device rd;
    mt19937 gen(rd());
	// For each query, generate 10 mutated versions with errors
    for (const string& original : queries) {
        for (int j = 0; j < 10; j++) {
            string mutated = makeWordWithErrors(original, K, gen);
            allWords.push_back(mutated);
        }
    }

    return allWords;
}

// Main function to generate text file with specified parameters
// sizeInMB: desired size of the text file in megabytes
// filename: name of the output file
//m: length of each word
//k: maximum number of errors
//s: number of queries
//queries: list of original words
// The function generates words with errors and creates a random text file
void generateText(double sizeInMB, const string& filename, int m ,int k ,int s, const vector<string>& queries)
{
	// Generate words with errors
    vector<string> words = generateSetOfWordsWithErrors(queries, k);

	// Create random text file with all words once
    createRandomText(words, sizeInMB, filename, m);
	cout << "Generated text file: " << filename << " of size " << sizeInMB << " MB\n";
}



// Naive search 
// Function to check if a word matches a segment with up to k errors
bool matchesWithErrors(const string& word, const string& segment, int k) {
    int errors = 0;
    // Compare characters of word and segment
    for (size_t i = 0; i < word.size(); ++i) {
        if (word[i] != segment[i]) {
            errors++;
            if (errors > k) return false;
        }
    }
    return true;
}


// Find all positions of a word in the given text allowing up to k errors
// text: the whole text to search in
// word: the target word to search for
// k: maximum allowed errors
vector<int> findWordWithErrors(const string& text, const string& word, int k) {
    vector<int> positions;
    int m = static_cast<int>(word.size());

    // Slide over the text to find matching segments
    for (size_t i = 0; i + m <= text.size(); ++i) {
        string segment = text.substr(i, m);
        if (matchesWithErrors(word, segment, k)) {
            positions.push_back(static_cast<int>(i));
        }
    }
    return positions;
}


// Find positions of multiple words in the given text allowing up to k errors
// text: the whole text to search in
// words: list of target words to search for
// k: maximum allowed errors
vector<WordPositions> findSetWordsNaiveSearch(const string& text, const vector<string>& words,int k) {
    vector<WordPositions> results;

    for (const string& word : words) {
        vector<int> positions = findWordWithErrors(text, word, k);
        results.push_back({ word, positions });
    }
    return results;
}




// build configurations general functions

// Function to generate all binary combinations of length m with k zeros
//m: length of the binary string
//k: number of zeros in the binary string
// Returns a vector of binary strings
vector<string> getBinaryCombinations(int m, int k) {
	// Create initial string with k '0's and (m-k) '1's
    string s(m, '1'); 
    for (int i = 0; i < k; i++) {
        s[i] = '0'; 
    }
	// Sort to ensure we start with the lexicographically smallest permutation
    vector<string> combinations;
    do {
        if (s[0] == '1')        
            combinations.push_back(s);
    } while (next_permutation(s.begin(), s.end()));
    return combinations;
}


// Function to build initial configurations
// that cover all combinations
// M: number of ones in each configuration
// combinations: list of binary combinations to cover
//  Returns a vector of initial configurations
vector<string> buildConfigurations(int M, const vector<string>& combinations)
{
    vector<string> configurations;

    // Build initial configurations
    for (const auto& combo : combinations) {
        bool exists = false;

        // Check if combination is already covered
        for (const auto& config : configurations) {
            if (configMatchescombinations(combo, config)) {
                exists = true;
                break;
            }
        }

        // If not covered, create new configuration
        if (!exists) {
            int onesCount = 0;
            int end = 0;
            for (; end < combo.size(); end++) {
                if (combo[end] == '1') onesCount++;
                if (onesCount == M) break;
            }
            if (onesCount < M) end = combo.size() - 1;
            string newConfig = combo.substr(0, end + 1);
            configurations.push_back(newConfig);
        }
    }
    return configurations;
}




// general functions for find the queries positions

// Function to check if a configuration matches a combination
// combo: binary string representing the combination
// config: binary string representing the configuration
// Returns true if the configuration matches the combination
bool configMatchescombinations(const string& combo, const string& config) {
    int figLen = config.size();
    int cLen = combo.size();

	// If configuration is longer than combination, it cannot match
    if (cLen < figLen) return false;

	// Slide configuration over combination to check for matches
    for (int i = 0; i <= cLen - figLen; i++) {
        bool match = true;
		// Check if configuration matches at position i
        for (int j = 0; j < figLen; j++) {
            if (config[j] == '1' && combo[i + j] != '1') {
                match = false;
                break;
            }
        }
        if (match) return true;
    }
    return false;
}

// Function to build matches based on configurations
// text: the input text to search
// configurations: list of binary configurations
// returnMap: flag to indicate if results should be returned as a map
// outList: optional output list to store matches
// Returns an unordered_map of matches if returnMap is true
unordered_map<string, vector<int>> buildMatches(const string& text, const vector<string>& configurations, bool returnMap, vector<string>* outList) {
    unordered_map<string, vector<int>> results;
    // Placeholder character for non-matching positions
    const char placeholder = '$';

    //Iterate over each configuration
    for (const string& cfg : configurations) {
        int len = cfg.size();
        if (len > (int)text.size()) continue;
        
        string out(len, placeholder);

        // Slide over the text to extract substrings
        for (int i = 0; i <= (int)text.size() - len; i++) {
            
            std::fill(out.begin(), out.end(), placeholder);
            
            for (int j = 0; j < len; j++) {
                if (cfg[j] == '1')
                    out[j] = text[i + j];
            }
            // Store results based on the returnMap flag
            if (returnMap) {
                results[out].push_back(i);
            }
            else if (outList) {
                outList->push_back(out);
            }
        }
    }
    // Return results if requested
    return results;
}


vector<string> buildPosMatches(const string& text, const vector<string>& configurations, int pos) {
	vector<string> results;
    // Placeholder character for non-matching positions
    const char placeholder = '$';

    //Iterate over each configuration
    for (const string& cfg : configurations) {
        int len = cfg.size();
        if (len + pos > (int)text.size()) continue;

		string out(len, placeholder);
        for (int j = 0; j < len; j++) {
            if (cfg[j] == '1')
                out[j] = text[pos + j];
        }
		results.push_back(out);

    }
    // Return results if requested
    return results;
}


// Function to process queries on a file using given configurations
// and find positions allowing up to k errors
// Returns a vector of WordPositionsSet for each query
vector<WordPositionsSet> findSetWods(const vector<string>& configurations, const vector<string>& queries, const string& text, const unordered_map<string, vector<int>>& results , int m, int M, int k)
{
    
    vector<WordPositionsSet> locationsBasic;

    // query processing
    for (const string& q : queries)
    {
        // build all possible patterns from the query
        vector<string> tempQueries;
        buildMatches(q, configurations, false, &tempQueries);

        int fullLen = q.size();

        WordPositionsSet matchResult;
        matchResult.word = q;

        // for each pattern generated from the query
        for (const string& queryStr : tempQueries)
        {
            auto it = results.find(queryStr);
            if (it == results.end())
                continue;

            int configLen = queryStr.size();

            // for each position where the pattern was found
            for (int idx : it->second)
            {
                // determine the range of possible alignments
                int start = max(0, idx - (fullLen - configLen));
                int end = min((int)text.size() - configLen, max(idx + fullLen - configLen, 0));

                // check all possible alignments
                for (int pos = start; pos <= end; pos++)
                {
                    if (matchResult.positions.count(pos))
                        continue;

                    if (pos < 0 || pos + fullLen >(int)text.size())
                        continue;

                    // count errors between the segment and the query
                    int errors = 0;
                    for (int i = 0; i < fullLen; i++) {
                        if (text[pos + i] != q[i]) {
                            errors++;
                            if (errors > k)
                                break;
                        }
                    }
                    // if errors are within the allowed limit, record the position
                    if (errors <= k) {
                        matchResult.positions.insert(pos);
                    }
                }
            }
        }
        // store the results for the current query
        locationsBasic.push_back(matchResult);
    }

    return locationsBasic;
}


vector <WordPositionsSet> fideWordsWhithPositions(const vector<vector<string>>& vecOfSet, const vector<string>& queries, const string& text, const unordered_map<string, vector<int>>& results , int M, int m, int k) {
    vector <WordPositionsSet> locations;
    
    int mismatchCount = 0;
	// query processing
    for (const string& q : queries) {

        WordPositionsSet matchResult;
        matchResult.word = q;

        for (int i =0 ; i< vecOfSet.size(); i++) {
			vector<string> tempQueries = buildPosMatches(q, vecOfSet[i], i);
            
            // for each pattern generated from the query
            for (const string& queryStr : tempQueries) {
				
                auto it = results.find(queryStr);
                if (it == results.end())
                    continue;

                int configLen = queryStr.size();
                for (int idx : it->second) {
                    mismatchCount = 0;
                    int startPos = idx - i;  

                    if (startPos < 0 || startPos + q.size() > text.size())
                        continue;

                    for (int j = 0; j < q.size(); j++) {
                        int pos = startPos + j; 
                        if (q[j] != text[pos]) {
                            mismatchCount++;
                            if (mismatchCount > k)
                                break;
                        }
                    }
                    if (mismatchCount <= k) {
                        matchResult.positions.insert(startPos);  
                    }
                }
            }
		}
        locations.push_back(matchResult);
    }      
    return locations;
}






// general functions for build configurations

// Function to build and reduce configurations
// that cover all combinations with minimal configurations
// Returns a vector of reduced configurations
vector<string> buildConfigurationsBasic(int M, int m, int k , const vector<string>& combinations, vector<string>& configurations) {

    vector<vector<int>> covers(combinations.size());

	// Reduce configurations
    while (true) {

        vector<vector<int>> covers(combinations.size());
		// Determine which configurations cover which combinations
        for (int ci = 0; ci < combinations.size(); ci++) {
            for (int fi = 0; fi < configurations.size(); fi++) {
                if (configMatchescombinations(combinations[ci], configurations[fi])) {
                    covers[ci].push_back(fi);
                }
            }
        }

		// Identify essential configurations
        vector<bool> essential(configurations.size(), false);

		// Mark configurations that are the only ones covering a combination as essential
        for (int ci = 0; ci < combinations.size(); ci++) {
            if (covers[ci].size() == 1) {
                essential[covers[ci][0]] = true;
            }
        }

		// Find a non-essential configuration to remove
        int removeIndex = -1;
        for (int fi = 0; fi < configurations.size(); fi++) {
            if (!essential[fi]) {
                removeIndex = fi;
                break;
            }
        }
		// If all configurations are essential, exit the loop
        if (removeIndex == -1) break;

		// Remove the identified non-essential configuration
        configurations.erase(configurations.begin() + removeIndex);
    }
    
    return configurations;
}


// Function to build weighted configurations
// that cover all combinations with minimal configurations
// Returns a vector of reduced configurations
vector<string> buildConfigurationsWeightedConfiguration(int M, int m, int k) {
    
	// Get all binary combinations of length m with k zeros
    vector<string> combinations = getBinaryCombinations(m, k);
    
    vector<string> configurations;
	set <string> configSet;
    int onesCount = 0;
    // Build initial configurations
    for (const auto& combo : combinations) {
        onesCount = 0;
        for (int i = 0; i < combo.size(); i++) {
            if (combo[i] == '1') {
                onesCount++;
                if (onesCount == M) {
                    string newConfig = combo.substr(0, i + 1);
                    configSet.insert(newConfig);
                    break;
                }
            }
        }
    }

    for (const auto& cfg : configSet) {
        configurations.push_back(cfg);
	}

	// Reduce configurations
	vector<string> reducedConfigurations;

    // index of combination
    int index = 0;

	// maximum coverage count
	int max = 0;

    // index of configuration with maximum coverage
	int maxIndex = 0;

    // index of configuration
    int indexconfig = 0;

	// While there are still combinations to cover
    while (!combinations.empty()) {     
        max = 0;		
		maxIndex = 0;
        indexconfig = 0;

        vector <vector<int>> coverage;

		// Determine coverage of each configuration
        for (auto& config : configurations) {
            
            vector<int> maches;
            maches.clear();
			index = 0;
			// Check which combinations are covered by the current configuration
            for (const auto& combo : combinations) {

                if (configMatchescombinations(combo, config)) {
                    maches.push_back(index);
                }
                index++;
            }
            index = 0;
			// Store coverage for the current configuration
            coverage.push_back(maches);
			// Update the configuration with maximum coverage
            if (maches.size() > max) {
                maxIndex = indexconfig;
				max = maches.size();
            }
            indexconfig++;
			
			
        }
		// Add the configuration with maximum coverage to the reduced set
        reducedConfigurations.push_back(configurations[maxIndex]);
        
		// Remove the selected configuration 
        if (maxIndex >= 0 && maxIndex < configurations.size()) {
            configurations.erase(configurations.begin() + maxIndex);
        }

        int i;
		// Remove covered combinations in reverse order to avoid index shifting
        for (i = coverage[maxIndex].size() - 1; i >= 0; --i) {
            int indexToRemove = coverage[maxIndex][i];
            if (indexToRemove >= 0 && indexToRemove < combinations.size()) {
                combinations.erase(combinations.begin() + indexToRemove);
            }
        }
    }
    return reducedConfigurations;
}


vector < vector <string>> CreateVecOfSets(int M, int m, int k) {
    
    vector<vector<string>> vecOfSet;

    // Get all binary combinations of length m with k zeros
    vector<string> combinations = getBinaryCombinations(m, k);
    
    // Build initial configurations
    vector<string> configurations = buildConfigurationsWeightedConfiguration(M, m, k);

    buildConfigurationsBasic(M, m, k, combinations, configurations);

    vecOfSet.push_back(configurations);

    int pos = 0;
	int count = 0;
  

    while (!combinations.empty()) {
        for (int idx = combinations.size() - 1; idx >= 0; --idx) {
            bool shouldErase = false;
            for (auto& config : configurations) {
				count = 0;
                for (int i = 0; i < config.size(); i++) {

                    if (i + pos >= combinations[idx].size())
						break;

                    if (config[i] == '1' && combinations[idx][i + pos] == '1') {
						count++;
                        if (count == M) {
                            shouldErase = true;
							
                            combinations.erase(combinations.begin() + idx);
                            break;
                        }        
                    }
                }
                if (shouldErase) break;
            }
        }
        pos++;
        if (combinations.empty()) break;
		// redose configurations
        buildConfigurationsBasic(M, m, k, combinations, configurations);

        vecOfSet.push_back(configurations);

    }
	return vecOfSet;
}


// בונה את כל ה-patterns (out) האפשריים שנגזרים מה-queries בעזרת הקונפיגורציות
// כל pattern הוא מחרוזת באורך config, עם אותיות מהמילה ו־'$' במקומות שלא נבחרו.
// התוצאה: סט של כל ה-patterns הרלוונטיים שמגיעים מה-queries.
unordered_set<string> buildAllowedPatternsFromQueries(const vector<string>& configurations, const vector<string>& queries)
{
    unordered_set<string> allowedPatterns;

    for (const string& q : queries) {
        vector<string> tempPatterns;
        buildMatches(q, configurations, /*returnMap=*/false, &tempPatterns);

        for (const string& pat : tempPatterns) {
            allowedPatterns.insert(pat);
        }
    }

    return allowedPatterns;
}


unordered_map<string, vector<int>> buildMatchesFiltered( const string& text, const vector<string>& configurations, const unordered_set<string>& allowedPatterns)
{
    unordered_map<string, vector<int>> results;
    const char placeholder = '$';

    if (text.empty() || configurations.empty() || allowedPatterns.empty()) {
        return results;
    }

    for (const string& cfg : configurations) {
        int len = static_cast<int>(cfg.size());
        if (len > static_cast<int>(text.size())) {
            continue;
        }

        string out(len, placeholder);

        for (int i = 0; i <= static_cast<int>(text.size()) - len; ++i) {

            std::fill(out.begin(), out.end(), placeholder);

            for (int j = 0; j < len; ++j) {
                if (cfg[j] == '1') {
                    out[j] = text[i + j];
                }
            }

            if (allowedPatterns.find(out) != allowedPatterns.end()) {
                results[out].push_back(i);
            }
        }
    }

    return results;
}








