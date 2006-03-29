<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<Project>

  <Description>&lt;h1&gt;Example project for the CLAM Descriptors Extractor&lt;/h1&gt;
&lt;p&gt;
&lt;em&gt;This is a generated project to test the CLAM music annotator.Descriptors have been generated by the program &lt;tt&gt;ClamExtractorExample&lt;/tt&gt; provided with the annotator.You could use that extractor as example to build your own one.&lt;/em&gt;&lt;/p&gt;
&lt;h2&gt;Description:&lt;/h2&gt;
&lt;p&gt;
In a real project this documentation may be useful for example to give instructions aboutthe annotation task or the aspects to take into account for the review.&lt;/p&gt;
&lt;h2&gt;Reference descriptors:&lt;/h2&gt;
&lt;p&gt;
You can take some descriptors as reference such as the energy.
Consider that such descriptors are computed every 2048 samples so their precission may be not as accurate as it seems.
&lt;/p&gt;
</Description>

  <Schema>Schema.sc</Schema>

  <Extractor>ClamExtractorExample</Extractor>

  <PoolSuffix>.pool</PoolSuffix>

  <Songs>
    <Song>
      <SoundFile>SongsTest/ChordProgressionAfter2000ms.wav</SoundFile>
    </Song>
    <Song>
      <SoundFile>SongsTest/LisaRein-SomethingBetter.mp3</SoundFile>
    </Song>
    <Song>
      <SoundFile>SongsTest/LisaRein-spunkyfunk.mp3</SoundFile>
    </Song>
  </Songs>

</Project>
