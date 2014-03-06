var fs = require('fs');
var Midi = require('jsmidgen');
var chordFile = new Midi.File();
var melodyFile = new Midi.File();
var chordTrack = new Midi.Track();
var melodyTrack = new Midi.Track();
chordFile.addTrack(chordTrack);
melodyFile.addTrack(melodyTrack);

//base ticks as per jsmidgen
var QUARTER_NOTE = 128;

//a measure is actually two measures, or one in 8/4 time
var MEASURE = QUARTER_NOTE * 8
  , WHOLE_NOTE = QUARTER_NOTE * 4
  , HALF_NOTE = QUARTER_NOTE * 2
  , EIGHTH_NOTE = QUARTER_NOTE / 2
  , SIXTEENTH_NOTE = QUARTER_NOTE / 4

//prob that the melody stops for a note
var pauseProbability = 0.2

var chordLengthProbabilities = [
   {l:EIGHTH_NOTE, chance: 0},
   {l:HALF_NOTE, chance: 40},
   {l:QUARTER_NOTE, chance:40},
   {l:WHOLE_NOTE, chance:20}
]

var noteLengthProbabilities = [
   {l:WHOLE_NOTE, chance: 0},
   {l:HALF_NOTE, chance: 20},
   {l:QUARTER_NOTE, chance:40},
   {l:EIGHTH_NOTE, chance: 40},
   {l:SIXTEENTH_NOTE, chance: 00}
]

//a chord goes to other chords and makes sound
var Chord = function(name, generateFunc, goesTo) {
   return {
      generate : generateFunc,
      goesTo   : goesTo
   }
}

//chords in C
var I    = new Chord('I')
,   II   = new Chord('II')
,   III  = new Chord('III')
,   IV   = new Chord('IV')
,   V    = new Chord('V')
,   VI   = new Chord('VI')
,   VII  = new Chord('VII')

//in array form~~
var chords = [I, II, III, IV, V, VI, VII];



//creates a song with ease, different seeds for different parts
var makeSong = function(verseSeed,chorusSeed, endSeed) {
   for (var i = 0; i < 2; i++) {
      generateChordMeasure('c',verseSeed + 10);
      generateSilence(MEASURE);
   }
   for (var i = 0; i < 2; i++) {
      generateChordMeasure('c',verseSeed + 10);
      generateMelodyMeasure('c5',verseSeed);
      generateChordMeasure('c',chorusSeed);
      generateMelodyMeasure('c5',chorusSeed);
   }
   for (var i = 0; i < 2; i++) {
      generateChordMeasure('a',chorusSeed);
      generateMelodyMeasure('a5',chorusSeed);
   }
   for (var i = 0; i < 1; i++) {
      generateChordMeasure('c',verseSeed + 10);
      generateMelodyMeasure('c5',verseSeed);
      generateChordMeasure('c',chorusSeed);
      generateMelodyMeasure('c5',chorusSeed);
   }
   for (var i = 0; i < 2; i++) {
      generateChordMeasure('c',endSeed);
      generateMelodyMeasure('c5',endSeed);
      generateChordMeasure('c',chorusSeed);
      generateMelodyMeasure('c5',chorusSeed);
   }
   generateChord(WHOLE_NOTE, 'c4','e4','g4');
   generateMelodyNote(WHOLE_NOTE, 'c5');
}


//generate a melody from a starting pitch, each note no more than a minor third away
var generateMelodyMeasure = function(startingPitch, seed) {
   var startingPitch = Midi.Util.ensureMidiPitch(startingPitch);
   console.log(startingPitch);
   var currentPitch = startingPitch;
   var theSeed = seed;
   for (var ticks = 0; ticks < MEASURE;) {
      var maybeSilent = randFromSeed(seed);
      var noteLength = randomLength(ticks, noteLengthProbabilities, theSeed);
      if (maybeSilent < pauseProbability) {
         generateSilence(noteLength);
      }
      else {
         generateMelodyNote(noteLength, currentPitch);
      }
      ticks += noteLength;
      currentPitch = findNextPitch(currentPitch, theSeed++);
   }
}

//a kind of pseudo-random number
var randFromSeed = function(seed) {
   var x = Math.sin(seed);
   return x - Math.floor(x);
}

//sounds like nothing over time
var generateSilence = function(length) {
   melodyTrack.addNoteOff(0,0,length);
}

//notes in a major scale
var pitchesInScale = [0,2,4,5,7,9,11];
   
//looks for the closest in-key pitch to a random number of half steps away
var findNextPitch = function(pitch, seed) {
   var interval = (Math.floor(randFromSeed(seed) * 100) % 5) - 2;
   pitch += interval;
   for (var i = 0; i < pitchesInScale.length; i++) {
      var note = Math.floor(pitch % 12);
      if (note == pitchesInScale[i] + 1) {
         pitch += pitchesInScale[i] - (pitch % 12);
      }
   }
   return pitch;
}



//these functions make sound for chords
I.generate = function(length) {
   generateChord(length,'c4','e4','g4');
};
II.generate = function(length) {
   generateChord(length,'d4','f4','a4');
};
III.generate = function(length) {
   generateChord(length,'e4','g4','b4');
};
IV.generate = function(length) {
   generateChord(length,'f4','a4','c4');
};
V.generate = function(length) {
   generateChord(length,'g4','b4','d4');
};
VI.generate = function(length) {
   generateChord(length,'a4','c4','e4');
};
VII.generate = function(length) {
   generateChord(length,'b3','d4','f4');
};

//goesTo contains the probability of one chord going to every other
I.goesTo = [{chord:I, chance:15},
            {chord:II, chance:15},
            {chord:III, chance:10}, 
            {chord:IV, chance:20}, 
            {chord:V, chance:20}, 
            {chord:VI, chance:10}, 
            {chord:VII, chance:10}];

II.goesTo = [{chord:I, chance:0},
            {chord:II, chance:10},
            {chord:III, chance:0}, 
            {chord:IV, chance:20}, 
            {chord:V, chance:70}, 
            {chord:VI, chance:0}, 
            {chord:VII, chance:0}];

III.goesTo = [{chord:I, chance:0},
            {chord:II, chance:0},
            {chord:III, chance:20}, 
            {chord:IV, chance:20}, 
            {chord:V, chance:10}, 
            {chord:VI, chance:50}, 
            {chord:VII, chance:0}];

IV.goesTo = [{chord:I, chance:20},
            {chord:II, chance:0},
            {chord:III, chance:0}, 
            {chord:IV, chance:30}, 
            {chord:V, chance:0}, 
            {chord:VI, chance:0}, 
            {chord:VII, chance:50}];

V.goesTo = [{chord:I, chance:60},
            {chord:II, chance:0},
            {chord:III, chance:0}, 
            {chord:IV, chance:0}, 
            {chord:V, chance:20}, 
            {chord:VI, chance:20}, 
            {chord:VII, chance:0}];

VI.goesTo = [{chord:I, chance:0},
            {chord:II, chance:40},
            {chord:III, chance:10}, 
            {chord:IV, chance:0}, 
            {chord:V, chance:0}, 
            {chord:VI, chance:10}, 
            {chord:VII, chance:30}];

VII.goesTo = [{chord:I, chance:50},
            {chord:II, chance:0},
            {chord:III, chance:50}, 
            {chord:IV, chance:0}, 
            {chord:V, chance:0}, 
            {chord:VI, chance:0}, 
            {chord:VII, chance:0}];


//which chord to play next?
var chooseNext = function(chord, seed) {
   var rand = Math.floor(randFromSeed(seed) * 100);
   console.log(rand);
   var percentCount = 0;
   for (var i = 0; i < chord.goesTo.length; i++) {
      if (rand < percentCount + chord.goesTo[i].chance) {
         console.log('wow');
         return chord.goesTo[i].chord;
      }
      else {
         console.log('trying again');
         percentCount+= chord.goesTo[i].chance;
         console.log(percentCount);
      }
   }
}


//write the chord in midi
var generateChord = function(length,noteNames) {
   for (var i = 1; i < arguments.length; i++) {
      var note = arguments[i];
      chordTrack.addNoteOn(0,note, 0);
   }
   chordTrack.addNoteOff(0,'c0',length);
   for (var i = 0; i < arguments.length; i++) {
      var note = arguments[i];
      chordTrack.addNoteOff(0,note,0);
   }
}

//write the midi notes
var generateMelodyNote = function(length,noteNames) {
   for (var i = 1; i < arguments.length; i++) {
      var note = arguments[i];
      melodyTrack.addNote(0, note, length);
   }
}

//generate a random length from prob (an array of objects of lengths and the probabilities of getting them)
var randomLength = function(ticks, prob, seed) {
   var ticksLeft = MEASURE - ticks;
   var rand = Math.floor(randFromSeed(seed) * 100);
   console.log(rand);
   var percentCount = 0;
   for (var i = 0; i < prob.length; i++) {
      console.log('p:'+prob[i].chance);
      if (rand < percentCount + prob[i].chance) {
         console.log('wow');
         return prob[i].l < ticksLeft ? prob[i].l : ticksLeft;
      }
      else {
         console.log('trying again');
         percentCount+= prob[i].chance;
         console.log(percentCount);
      }
   }
   return ticksLeft;
}


//put a few chords together to make a randomly generated measure
var generateChordMeasure = function(firstKey, seed) {
   var chord = chordOf(firstKey);
   console.log(chord);
   var theSeed = seed;
   for (var tick = 0; tick < MEASURE;) {
      var noteLength = randomLength(tick, chordLengthProbabilities, theSeed++);
      chord.generate(noteLength);
      chord = chooseNext(chord, theSeed);
      tick += noteLength;
   }
}

//convert a name to a chord
var chordOf = function(keyName) {
   switch (keyName) {
      case 'a': return VI;    break;
      case 'A': return VI;    break;
      case 'b': return VII;   break;
      case 'A': return VII;   break;
      case 'c': return I;     break;
      case 'C': return I;     break;
      case 'd': return II;    break;
      case 'D': return II;    break;
      case 'e': return III;   break;
      case 'E': return III;   break;
      case 'f': return IV;    break;
      case 'F': return IV;    break;
      case 'g': return V;     break;
      case 'G': return V;     break;
   }
}



//lets run this puppy
makeSong(786,992,14131412);

//and save the midi file
fs.writeFileSync('chord.mid', chordFile.toBytes(), 'binary');
fs.writeFileSync('melody.mid', melodyFile.toBytes(), 'binary');
